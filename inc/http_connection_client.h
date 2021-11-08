//
// Created by ytymchenko on 15.10.2021.
//

#ifndef LOGGIO_HTTP_CONNECTION_CLIENT_H
#define LOGGIO_HTTP_CONNECTION_CLIENT_H

#include "http_connection.h"
#include "logger_client.h"

class http_connection_client : public http_connection {
public:
    explicit http_connection_client(asio::thread_pool &pool_, tcp::socket
    socket, concurrent_map<std::string>& m, std::vector<logger_client> &clients)
            : http_connection(std::move(socket), m), clients(clients), pool_(pool_) {}

protected:
    std::vector<logger_client> &clients;
    asio::thread_pool &pool_;

    void process_request() override;

    void process_post();


};


void http_server(tcp::acceptor &acceptor,asio::thread_pool &pool_, tcp::socket &socket, concurrent_map<std::string>& m,
                 std::vector<logger_client> &clients);


#endif //LOGGIO_HTTP_CONNECTION_CLIENT_H
