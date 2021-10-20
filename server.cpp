#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include "http_connection_server.h"

#include <cstdlib>


#include "logger_server.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;


int main() {
    try {
        logger_server service;
        const std::string addr = "0.0.0.0:50051";
        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        ServerBuilder builder;
        builder.AddListeningPort(addr, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << addr << std::endl;

        asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(),
                                                                   8080}};
        std::cout << "Running on http://" << acceptor.local_endpoint().address() << ":"
                  << acceptor.local_endpoint().port();

        tcp::socket socket{ioc};
        http_server(acceptor, socket, service.list);
        std::thread thread([&ioc]() { ioc.run(); });
        server->Wait();

        thread.join();
    }
    catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return 0;
}
