#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include "http_connection_server.h"

#include <cstdlib>
#include <cli.h>


#include "logger_server.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;


int main(int argc, char *argv[]) {
    try {
        cli cli_ob{argc, argv};
        logger_server service;

        grpc::EnableDefaultHealthCheckService(true);
        grpc::reflection::InitProtoReflectionServerBuilderPlugin();
        ServerBuilder builder;
        builder.AddListeningPort(cli_ob.get_address(), grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<Server> server(builder.BuildAndStart());
        std::cout << "Server listening on " << cli_ob.get_address() << std::endl;

        std::cout << std::endl;

        asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(),
                                                                   cli_ob.get_port()}};
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
