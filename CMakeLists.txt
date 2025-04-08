cmake_minimum_required(VERSION 3.10)
project(Mini2)

set(Python3_EXECUTABLE "/usr/local/bin/python3.12" CACHE STRING "Python 3.12 executable")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python interpreter for proto generation
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Add this line to help find abseil
set(ABSL_PROPAGATE_CXX_STD ON)

# Add after project(Mini2)
set(ABSL_USE_EXTERNAL_GOOGLETEST OFF)
set(BUILD_TESTING OFF)

# Add this to ensure proper linking
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Add these lines
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed")

# Find required packages
find_package(Threads REQUIRED)
find_package(absl CONFIG REQUIRED)
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Find gRPC executables
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin REQUIRED)
find_program(PROTOC protoc REQUIRED)

# Generate protobuf and gRPC code
get_filename_component(mini2_proto "proto/mini2.proto" ABSOLUTE)
get_filename_component(mini2_proto_path "${mini2_proto}" PATH)

# Generate C++ files 
set(mini2_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/proto/mini2.pb.cc")
set(mini2_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/proto/mini2.pb.h")
set(mini2_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/proto/mini2.grpc.pb.cc")
set(mini2_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/proto/mini2.grpc.pb.h")

# Generate Python files
set(PYTHON_PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/python/src")
file(MAKE_DIRECTORY ${PYTHON_PROTO_DIR})
set(python_proto_srcs
    "${PYTHON_PROTO_DIR}/mini2_pb2.py"
    "${PYTHON_PROTO_DIR}/mini2_pb2_grpc.py"
)

# Add protobuf and gRPC targets
add_custom_command(
    OUTPUT 
        "${mini2_proto_srcs}"
        "${mini2_proto_hdrs}"
        "${mini2_grpc_srcs}"
        "${mini2_grpc_hdrs}"
    COMMAND ${PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/proto"
         --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}/cpp/include/proto"
         -I "${CMAKE_CURRENT_SOURCE_DIR}/proto"
         --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
         "${CMAKE_CURRENT_SOURCE_DIR}/proto/mini2.proto"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/proto/mini2.proto"
)

# Add Python proto generation
add_custom_command(
    OUTPUT ${python_proto_srcs}
    COMMAND ${Python3_EXECUTABLE}
    ARGS -m grpc_tools.protoc
         -I "${CMAKE_CURRENT_SOURCE_DIR}/proto"
         --python_out="${PYTHON_PROTO_DIR}"
         --grpc_python_out="${PYTHON_PROTO_DIR}"
         "${CMAKE_CURRENT_SOURCE_DIR}/proto/mini2.proto"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/proto/mini2.proto"
)

# Combined generation target
add_custom_target(generate_proto_files
    DEPENDS
        "${mini2_proto_srcs}"
        "${mini2_proto_hdrs}"
        "${mini2_grpc_srcs}"
        "${mini2_grpc_hdrs}"
        ${python_proto_srcs}
)

# Server source files
set(SERVER_SRC
    cpp/src/server.cpp
    cpp/src/SpatialAnalysis.cpp
    ${mini2_proto_srcs}
    ${mini2_grpc_srcs}
)

add_executable(server ${SERVER_SRC})
add_dependencies(server generate_proto_files)

# Link libraries
target_link_libraries(server PRIVATE
    gRPC::grpc++
    gRPC::grpc++_reflection
    protobuf::libprotobuf
    Threads::Threads
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include
    ${Protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
    ${ABSL_INCLUDE_DIRS}
)
