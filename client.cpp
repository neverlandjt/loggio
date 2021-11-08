//
// Created by ytymchenko on 15.10.2021.
//

#include "http_connection_client.h"
#include "logger_client.h"


#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

void initialize_clients(std::vector<logger_client> &clients) {
    std::vector<std::string> addresses;
    boost::split(addresses, std::string(getenv("ADDRESSES")), boost::is_any_of(";"));

    for (auto &address: addresses)
        clients.emplace_back(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));

}

int main() {
    try {
        std::vector<logger_client> clients;
        asio::thread_pool pool(4);

        initialize_clients(clients);
        asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc,
                               boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(),
                                                              8080}};
        std::cout << "Running on http://" << acceptor.local_endpoint().address() << ":"
                  << acceptor.local_endpoint().port() << std::endl;
        concurrent_map<std::string> m;
        tcp::socket socket{ioc};
        http_server(acceptor, pool, socket, m, clients);
        std::thread thread([&ioc]() { ioc.run(); });
        thread.join();


    }
    catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

