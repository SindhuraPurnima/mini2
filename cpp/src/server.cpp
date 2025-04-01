#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sys/shm.h>  // For shared memory
#include <nlohmann/json.hpp>  // JSON parsing library
#include "proto/mini2.grpc.pb.h"
#include "proto/mini2.pb.h"
#include "parser/CSV.h"

using json = nlohmann::json;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ClientContext;
using grpc::Channel;
using mini2::CollisionData;
using mini2::CollisionBatch;
using mini2::RiskAssessment;
using mini2::Empty;
using mini2::EntryPointService;
using mini2::InterServerService;

// Structure to hold shared memory information
struct SharedMemorySegment {
    int shmid;
    void* memory;
    size_t size;
};

// Structure to represent a process node in the overlay network
struct ProcessNode {
    std::string id;
    std::string address;
    int port;
    std::vector<std::string> connections;
    SharedMemorySegment shm;
};

// Structure for shared memory control
struct SharedMemoryControl {
    int write_index;
    int read_index;
    int data_count;
    // Additional control fields can be added here
};

// Generic Server implementation
class GenericServer : public EntryPointService::Service, public InterServerService::Service {
private:
    // Server configuration
    std::string server_id;
    std::string server_address;
    int server_port;
    
    // Overlay network configuration
    std::map<std::string, ProcessNode> network_nodes;
    std::vector<std::string> connections;
    
    // Shared memory segments (one for each connection)
    std::map<std::string, SharedMemorySegment> shared_memories;
    
    // gRPC stubs for connections to other servers
    std::map<std::string, std::unique_ptr<InterServerService::Stub>> server_stubs;
    
    // Data processing state
    bool is_entry_point;
    
    // Initialize shared memory for a specific connection
    bool initSharedMemory(const std::string& connection_id, int key, size_t size) {
        SharedMemorySegment shm;
        
        // Create shared memory segment
        shm.size = size;
        shm.shmid = shmget(key, size + sizeof(SharedMemoryControl), IPC_CREAT | 0666);
        if (shm.shmid < 0) {
            std::cerr << "Failed to create shared memory segment for " << connection_id << std::endl;
            return false;
        }
        
        // Attach to shared memory
        shm.memory = shmat(shm.shmid, NULL, 0);
        if (shm.memory == (void*)-1) {
            std::cerr << "Failed to attach to shared memory for " << connection_id << std::endl;
            return false;
        }
        
        // Initialize control structure
        SharedMemoryControl* control = static_cast<SharedMemoryControl*>(shm.memory);
        control->write_index = 0;
        control->read_index = 0;
        control->data_count = 0;
        
        // Add to shared memory map
        shared_memories[connection_id] = shm;
        
        std::cout << "Initialized shared memory for connection to " << connection_id << std::endl;
        return true;
    }
    
    // Initialize gRPC channel to another server
    void initServerStub(const std::string& server_id) {
        if (network_nodes.find(server_id) != network_nodes.end()) {
            std::string target_address = network_nodes[server_id].address + ":" + 
                                        std::to_string(network_nodes[server_id].port);
            auto channel = grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials());
            server_stubs[server_id] = InterServerService::NewStub(channel);
            std::cout << "Created channel to server " << server_id << " at " << target_address << std::endl;
        }
    }
    
    // Forward data to connected servers via gRPC
    void forwardDataToServer(const std::string& server_id, const CollisionBatch& batch) {
        if (server_stubs.find(server_id) == server_stubs.end()) {
            initServerStub(server_id);
        }
        
        ClientContext context;
        Empty response;
        Status status = server_stubs[server_id]->ForwardData(&context, batch, &response);
        
        if (!status.ok()) {
            std::cerr << "Failed to forward data to " << server_id << ": " 
                      << status.error_message() << std::endl;
        }
    }
    
    // Write data to shared memory for a specific connection
    bool writeToSharedMemory(const std::string& connection_id, const CollisionData& data) {
        if (shared_memories.find(connection_id) == shared_memories.end()) {
            std::cerr << "Shared memory for " << connection_id << " not initialized" << std::endl;
            return false;
        }
        
        SharedMemorySegment& shm = shared_memories[connection_id];
        SharedMemoryControl* control = static_cast<SharedMemoryControl*>(shm.memory);
        
        // Check if there's space in the buffer
        if (control->data_count >= (shm.size / sizeof(CollisionData))) {
            std::cerr << "Shared memory buffer full for " << connection_id << std::endl;
            return false;
        }
        
        // Get pointer to data area (after control structure)
        char* data_area = static_cast<char*>(shm.memory) + sizeof(SharedMemoryControl);
        
        // Serialize the data to the appropriate position
        std::string serialized_data;
        data.SerializeToString(&serialized_data);
        
        // Copy serialized data to shared memory
        // In a real implementation, you would need a more sophisticated buffer management
        // This is a simplistic example
        std::memcpy(data_area + control->write_index * sizeof(CollisionData), 
                   serialized_data.data(), 
                   std::min(serialized_data.size(), sizeof(CollisionData)));
        
        // Update control structure
        control->write_index = (control->write_index + 1) % (shm.size / sizeof(CollisionData));
        control->data_count++;
        
        return true;
    }
    
    // Read data from shared memory for a specific connection
    bool readFromSharedMemory(const std::string& connection_id, CollisionData& data) {
        if (shared_memories.find(connection_id) == shared_memories.end()) {
            std::cerr << "Shared memory for " << connection_id << " not initialized" << std::endl;
            return false;
        }
        
        SharedMemorySegment& shm = shared_memories[connection_id];
        SharedMemoryControl* control = static_cast<SharedMemoryControl*>(shm.memory);
        
        // Check if there's data to read
        if (control->data_count == 0) {
            return false;
        }
        
        // Get pointer to data area (after control structure)
        char* data_area = static_cast<char*>(shm.memory) + sizeof(SharedMemoryControl);
        
        // Read serialized data from the appropriate position
        std::string serialized_data;
        serialized_data.resize(sizeof(CollisionData));
        std::memcpy(serialized_data.data(), 
                   data_area + control->read_index * sizeof(CollisionData),
                   sizeof(CollisionData));
        
        // Deserialize the data
        data.ParseFromString(serialized_data);
        
        // Update control structure
        control->read_index = (control->read_index + 1) % (shm.size / sizeof(CollisionData));
        control->data_count--;
        
        return true;
    }
    
    // Choose which server to forward data to
    std::string chooseTargetServer(const CollisionData& data) {
        // Implement your routing logic here
        // This is a simple round-robin example
        static size_t next_server_index = 0;
        
        if (connections.empty()) {
            return "";
        }
        
        std::string target = connections[next_server_index];
        next_server_index = (next_server_index + 1) % connections.size();
        
        return target;
    }

public:
    GenericServer(const std::string& config_path) : is_entry_point(false) {
        // Load configuration from JSON file
        std::ifstream config_file(config_path);
        if (!config_file.is_open()) {
            std::cerr << "Failed to open config file: " << config_path << std::endl;
            exit(1);
        }
        
        json config;
        config_file >> config;
        
        // Parse server configuration
        server_id = config["server_id"];
        server_address = config["address"];
        server_port = config["port"];
        is_entry_point = config["is_entry_point"];
        
        std::cout << "Configuring server " << server_id 
                  << " at " << server_address << ":" << server_port << std::endl;
        
        // Parse network configuration
        for (const auto& node : config["network"]) {
            ProcessNode process_node;
            process_node.id = node["id"];
            process_node.address = node["address"];
            process_node.port = node["port"];
            
            // Parse connections for this node
            for (const auto& conn : node["connections"]) {
                process_node.connections.push_back(conn);
            }
            
            network_nodes[process_node.id] = process_node;
            
            // If this is the current server, set up its connections
            if (process_node.id == server_id) {
                connections = process_node.connections;
            }
        }
        
        // Initialize shared memory for each connection
        int base_key = 1000;  // Starting key for shared memory
        for (size_t i = 0; i < connections.size(); i++) {
            // Use a different key for each connection
            int key = base_key + i;
            // 1MB shared memory segment for each connection
            if (!initSharedMemory(connections[i], key, 1024 * 1024)) {
                std::cerr << "Failed to initialize shared memory for " << connections[i] << std::endl;
                exit(1);
            }
        }
        
        // Initialize gRPC stubs for all connections
        for (const auto& conn : connections) {
            initServerStub(conn);
        }
    }
    
    ~GenericServer() {
        // Clean up shared memory
        for (auto& shm_pair : shared_memories) {
            if (shm_pair.second.memory != nullptr && shm_pair.second.memory != (void*)-1) {
                shmdt(shm_pair.second.memory);
                shmctl(shm_pair.second.shmid, IPC_RMID, NULL);
            }
        }
    }
    
    // Handle incoming collision data from Python client (entry point)
    Status StreamCollisions(ServerContext* context,
                          grpc::ServerReader<CollisionData>* reader,
                          Empty* response) override {
        // Only process this if the server is an entry point
        if (!is_entry_point) {
            return Status(grpc::StatusCode::FAILED_PRECONDITION, 
                         "This server is not configured as an entry point");
        }
        
        CollisionData collision;
        int count = 0;
        
        // Read streaming data from client
        while (reader->Read(&collision)) {
            count++;
            
            // Log progress
            if (count % 1000 == 0) {
                std::cout << "Received " << count << " records" << std::endl;
            }
            
            // Determine which server to route this data to
            std::string target_server = chooseTargetServer(collision);
            
            if (!target_server.empty()) {
                // Try to write to shared memory first
                if (writeToSharedMemory(target_server, collision)) {
                    std::cout << "Data written to shared memory for " << target_server << std::endl;
                } else {
                    // If shared memory fails, fall back to gRPC
                    CollisionBatch batch;
                    *batch.add_collisions() = collision;
                    forwardDataToServer(target_server, batch);
                }
            }
        }
        
        std::cout << "Finished receiving " << count << " records" << std::endl;
        return Status::OK;
    }
    
    // Handle forwarded data from other servers
    Status ForwardData(ServerContext* context,
                      const CollisionBatch* batch,
                      Empty* response) override {
        std::cout << "Received forwarded batch with " << batch->collisions_size() << " records" << std::endl;
        
        // Process each collision in the batch
        for (int i = 0; i < batch->collisions_size(); i++) {
            const CollisionData& collision = batch->collisions(i);
            
            // Process the data locally
            // ...
            
            // Potentially forward to other servers based on your routing logic
            std::string target_server = chooseTargetServer(collision);
            
            if (!target_server.empty()) {
                // Try to write to shared memory first
                if (writeToSharedMemory(target_server, collision)) {
                    std::cout << "Data written to shared memory for " << target_server << std::endl;
                } else {
                    // If shared memory fails, fall back to gRPC
                    CollisionBatch new_batch;
                    *new_batch.add_collisions() = collision;
                    forwardDataToServer(target_server, new_batch);
                }
            }
        }
        
        return Status::OK;
    }
    
    // Handle sharing of analysis results
    Status ShareAnalysis(ServerContext* context,
                        const RiskAssessment* assessment,
                        Empty* response) override {
        std::cout << "Received risk assessment for " << assessment->borough() 
                  << " " << assessment->zip_code() << std::endl;
        
        // Process the assessment data
        // ...
        
        return Status::OK;
    }
    
    // Get server address (IP:port)
    std::string getServerAddress() const {
        return server_address + ":" + std::to_string(server_port);
    }
    
    // Check if this server is an entry point
    bool isEntryPoint() const {
        return is_entry_point;
    }
};

int main(int argc, char** argv) {
    // Check for config file path
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file.json>" << std::endl;
        return 1;
    }
    
    std::string config_path = argv[1];
    
    // Create and configure the server
    GenericServer server(config_path);
    
    // Set up gRPC server
    ServerBuilder builder;
    builder.AddListeningPort(server.getServerAddress(), grpc::InsecureServerCredentials());

    // Explicitly cast to resolve the ambiguity
    if (server.isEntryPoint()) {
        builder.RegisterService(static_cast<EntryPointService::Service*>(&server));
    }
    builder.RegisterService(static_cast<InterServerService::Service*>(&server));
    
    // Start the server
    std::unique_ptr<Server> grpc_server(builder.BuildAndStart());
    std::cout << "Server " << (server.isEntryPoint() ? "(entry point) " : "") 
              << "listening on " << server.getServerAddress() << std::endl;
    
    // Wait for server to finish
    grpc_server->Wait();
    
    return 0;
}