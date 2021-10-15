//
// Created by ytymchenko on 15.10.2021.
//

#include "http_connection_client.h"
#include "logger_client.h"


#include <cstdlib>
#include <iostream>
#include <string>
#include <cli.h>


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

void initialize_clients(const std::vector<std::string>& addresses, std::vector<logger_client>& clients){
    for (auto & address: addresses)
        clients.emplace_back(  grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));

}

int main(int argc, char *argv[]) {
    try {
        cli cli_ob{argc, argv};
        std::vector<logger_client> clients;
        initialize_clients(cli_ob.get_addresses(), clients);
        asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc,
                               boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(),
                                                              8081}};
        std::cout << "Running on http://" << acceptor.local_endpoint().address() << ":"
                  << acceptor.local_endpoint().port() << std::endl;
        std::vector<std::string> v;
        tcp::socket socket{ioc};
        http_server(acceptor, socket, v, clients);
        std::thread thread([&ioc]() { ioc.run(); });
        thread.join();

    }
    catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

