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


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

void initialize_clients(std::vector<std::shared_ptr<logger_client>> &clients) {
    std::vector<std::string> addresses;
    boost::split(addresses, std::string(getenv("ADDRESSES")), boost::is_any_of(";"));

    for (size_t i = 0; i < addresses.size(); ++i) {
        auto c = std::make_shared<logger_client>(grpc::CreateChannel(addresses[i],
                                                                     grpc::InsecureChannelCredentials()), i);
        clients.push_back(c);
    }

}

[[noreturn]] void health_check(std::vector<std::shared_ptr<logger_client>> &clients) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        for (auto &client: clients) client->HealthCheck();
    }
}


int main() {
    try {
        std::vector<std::shared_ptr<logger_client>> clients;

        initialize_clients(clients);
        asio::io_context ioc{2};

        tcp::acceptor acceptor{ioc,
                               boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(),
                                                              8080}};
        std::cout << "Running on http://" << acceptor.local_endpoint().address() << ":"
                  << acceptor.local_endpoint().port() << std::endl;

        concurrent_map<std::string> message_map;
        http_server(acceptor, message_map, clients);

        std::vector<std::thread> threads_io;
        std::vector<std::thread> threads_clients;

        uint8_t threads_number_io = 2;

        threads_io.reserve(threads_number_io);
        threads_clients.reserve(clients.size());

        for (uint8_t i = 0; i < threads_number_io; ++i) threads_io.emplace_back([&ioc]() { ioc.run(); });
        for (auto &client : clients) threads_clients.emplace_back([&client]() { client->UpdateList(); });
        std::thread health_check_thread([&clients]() { health_check(clients); });

        for (auto &th: threads_io) th.join();
        for (auto &th: threads_io) th.join();
        health_check_thread.join();

    }
    catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

