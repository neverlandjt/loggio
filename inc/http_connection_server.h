//
// Created by ytymchenko on 15.10.2021.
//

#ifndef LOGGIO_HTTP_CONNECTION_SERVER_H
#define LOGGIO_HTTP_CONNECTION_SERVER_H

#include "http_connection.h"

class http_connection_server : public http_connection {
public:
    explicit http_connection_server(tcp::socket socket, std::vector<std::string> &v)
            : http_connection(std::move(socket), v) {}

};

void http_server(tcp::acceptor &acceptor, tcp::socket &socket, std::vector<std::string> &v);
#endif //LOGGIO_HTTP_CONNECTION_SERVER_H
