//
// Created by ytymchenko on 15.10.2021.
//

#ifndef LOGGIO_HTTP_CONNECTION_CLIENT_H
#define LOGGIO_HTTP_CONNECTION_CLIENT_H

#include "http_connection.h"
#include "logger_client.h"

class http_connection_client : public http_connection {
public:
    explicit http_connection_client( tcp::socket
    socket, concurrent_map<std::string> &m, std::vector<std::shared_ptr<logger_client>> &clients)
            : http_connection(std::move(socket), m), clients(clients) {}

protected:
    std::vector<std::shared_ptr<logger_client>> &clients;

    void process_request() override;

    void process_post();

    void process_get_health();

    bool check_quorum();


};


void http_server(tcp::acceptor &acceptor,
                 concurrent_map<std::string> &message_map_,
                 std::vector<std::shared_ptr<logger_client>> &clients);


#endif //LOGGIO_HTTP_CONNECTION_CLIENT_H
