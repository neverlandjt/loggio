//
// Created by ytymchenko on 15.10.2021.
//

#include <http_connection_server.h>

void http_server(tcp::acceptor &acceptor, tcp::socket &socket, concurrent_map<std::string>& m) {
    acceptor.async_accept(socket,
                          [&](beast::error_code ec) {
                              if (!ec)
                                  std::make_shared<http_connection_server>(std::move(socket), m)->start();
                              http_server(acceptor, socket, m);
                          });
}


