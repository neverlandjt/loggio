cmake_minimum_required(VERSION 3.15)

# Project
project(loggio)

# Protobuf
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

# Protobuf-compiler
set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
set(_GRPC_GRPCPP gRPC::grpc++)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

# Proto file
get_filename_component(hw_proto "./protos/logger.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

find_package(Boost REQUIRED system  program_options )


# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/logger.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/logger.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/logger.grpc.pb.cc" )
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/logger.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${Boost_INCLUDE_DIRS} inc/ src/)
link_directories(${Boost_LIBRARY_DIRS})

# Targets (client|server)
foreach(_target
        client server)
    add_executable(${_target} "inc/logger_${_target}.h" "src/logger_${_target}.cpp"  "${_target}.cpp"
            ${hw_proto_srcs}
            ${hw_grpc_srcs} inc/http_connection.h  inc/utilities.h   inc/http_connection_${_target}.h
           src/http_connection.cpp src/http_connection_${_target}.cpp src/latch.cpp inc/latch.h  inc/concurrent_map.h inc/concurrent_queue.h)
    target_link_libraries(${_target}
            ${_REFLECTION}
            ${_GRPC_GRPCPP}
            ${_PROTOBUF_LIBPROTOBUF} gRPC::grpc++_reflection ${Boost_LIBRARIES}  boost_system pthread dl ssl)
endforeach()


