#include <grpcpp/grpcpp.h>
#include <memory>
#include <string>
#include <sys/shm.h>  // For shared memory
#include "proto/mini2.grpc.pb.h"
#include "proto/mini2.pb.h"
#include "parser/CSV.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using mini2::CollisionData;
using mini2::CollisionBatch;
using mini2::Empty;
using mini2::EntryPointService;
using mini2::InterServerService;

// Server A implementation
class ServerA final : public EntryPointService::Service {
private:
    // Shared memory key for communication with Server B
    const int SHM_KEY = 1234;
    // Shared memory ID
    int shmid;
    // Pointer to shared memory
    void* shared_memory;

    // Initialize shared memory
    bool initSharedMemory() {
        // Create shared memory segment
        shmid = shmget(SHM_KEY, 1024, IPC_CREAT | 0666);
        if (shmid < 0) {
            std::cerr << "Failed to create shared memory segment" << std::endl;
            return false;
        }

        // Attach to shared memory
        shared_memory = shmat(shmid, NULL, 0);
        if (shared_memory == (void*)-1) {
            std::cerr << "Failed to attach to shared memory" << std::endl;
            return false;
        }

        return true;
    }

public:
    ServerA() {
        // Initialize shared memory when server starts
        if (!initSharedMemory()) {
            std::cerr << "Failed to initialize shared memory" << std::endl;
            exit(1);
        }
    }

    ~ServerA() {
        // Detach from shared memory
        shmdt(shared_memory);
        // Remove shared memory segment
        shmctl(shmid, IPC_RMID, NULL);
    }

    // Handle incoming collision data from Python client
    Status StreamCollisions(ServerContext* context,
                          grpc::ServerReader<CollisionData>* reader,
                          Empty* response) override {
        CollisionData collision;
        int count = 0;

        // Read streaming data from client
        while (reader->Read(&collision)) {
            count++;
            
            // Process the collision data
            if (count % 1000 == 0) {
                std::cout << "Received " << count << " records" << std::endl;
            }

            // Store data in shared memory for Server B
            // TODO: Implement proper data structure for shared memory
            
            // Forward data to other servers based on overlay network
            forwardToServers(collision);
        }

        std::cout << "Finished receiving " << count << " records" << std::endl;
        return Status::OK;
    }

private:
    // Forward data to other servers based on overlay network
    void forwardToServers(const CollisionData& collision) {
        // Create stubs for other servers
        // TODO: Implement forwarding logic based on overlay network
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ServerA service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server A listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
