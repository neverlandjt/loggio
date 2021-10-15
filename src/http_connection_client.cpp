//
// Created by ytymchenko on 15.10.2021.
//

#include <http_connection_client.h>
#include "utilities.h"


void http_connection_client::process_post() {
    response_.set(beast::http::field::content_type, "text/html");
    beast::ostream(response_.body()) << request_.body();

    if (sync_nodes(clients, request_.body(), v))
        v.emplace_back(request_.body());
}

void http_connection_client::process_request() {
    response_.version(request_.version());
    response_.keep_alive(false);

    switch (request_.method()) {
        case beast::http::verb::get:
            response_.result(beast::http::status::ok);
            response_.set(beast::http::field::server, "Beast");
            process_get();
            break;

        case beast::http::verb::post:
            response_.result(beast::http::status::ok);
            response_.set(beast::http::field::server, "Beast");
            process_post();
            break;

        default:
            response_.result(beast::http::status::bad_request);
            response_.set(beast::http::field::content_type, "text/plain");
            beast::ostream(response_.body())
                    << "Invalid request-method '"
                    << std::string(request_.method_string())
                    << "'";
            break;
    }
    response_.content_length(response_.body().size());
    write_response();
}

void http_server(tcp::acceptor &acceptor, tcp::socket &socket, std::vector<std::string> &v,
                 std::vector<logger_client> &clients) {
    acceptor.async_accept(socket,
                          [&](beast::error_code ec) {
                              if (!ec)
                                  std::make_shared<http_connection_client>(std::move(socket), v, clients)->start();
                              http_server(acceptor, socket, v, clients);
                          });
}
