//
// Created by ytymchenko on 15.10.2021.
//

#include <http_connection_client.h>
#include "utilities.h"
#include "latch.h"
#include <iostream>

#include <string>
#include <thread>


void http_connection_client::process_post() {
    ptree request_map = parse_request(request_.body());
    const std::string &message = request_map.get<std::string>("message");
    const int &write_concern = request_map.get<int>("write_concern");

    response_.set(beast::http::field::content_type, "text/html");
    beast::ostream(response_.body()) << message;
    std::shared_ptr<latch> latch_(new latch(write_concern > 0 ? write_concern : 1));

    for (auto &client : clients)
        asio::post(pool_, [&client, message, &latch_] {
            return sync_nodes(client, message, latch_);
        });

    latch_->count_down();
    latch_->wait();
    m.push(message);
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

void http_server(tcp::acceptor &acceptor, asio::thread_pool &pool_, tcp::socket &socket, concurrent_map<std::string>& m,
                 std::vector<logger_client> &clients) {
    acceptor.async_accept(socket,
                          [&](beast::error_code ec) {
                              if (!ec)
                                  std::make_shared<http_connection_client>(pool_, std::move(socket), m,
                                                                           clients)->start();
                              http_server(acceptor, pool_, socket, m, clients);
                          });
}
