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
    std::shared_ptr <latch> latch_(new latch(write_concern > 0 ? write_concern : 1));
    LoggerRequest request;
    request.set_idx(m.push(message) + 1);
    request.set_message(message);
    for (auto &client : clients)
        client->pending_q.push_back(q_type{request, latch_});

    latch_->count_down();
    latch_->wait();

}

void http_connection_client::process_request() {
    response_.version(request_.version());
    response_.keep_alive(false);

    switch (request_.method()) {
        case beast::http::verb::get:
            response_.result(beast::http::status::ok);
            response_.set(beast::http::field::server, "Beast");
            if (request_.target() == "/health") process_get_health();
            else process_get();
            break;

        case beast::http::verb::post:
            response_.result(beast::http::status::ok);
            response_.set(beast::http::field::server, "Beast");
            if (check_quorum()) process_post();
            else beast::ostream(response_.body()) << "No Quorum";
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

void http_connection_client::process_get_health() {
    response_.set(beast::http::field::content_type, "text/html");
    std::stringstream ss;
    ss << "Secondary nodes health status:<br>";
    for (const auto &item : clients)
        ss << "Node " << item->id << ": " << (item->healthy ? "Healthy<br>" : "Unhealthy<br>");
    beast::ostream(response_.body()) << ss.str();
}

bool http_connection_client::check_quorum() {
    size_t healthy = 1;
    for (auto &client: clients) healthy = healthy + client->healthy;
    std::cout << healthy << (clients.size()+ 1) / 2 + 1 << std::endl;
    return ((clients.size() + 1) / 2 + 1) <= healthy;
}

void http_server(tcp::acceptor &acceptor, concurrent_map <std::string> &message_map_,
                 std::vector <std::shared_ptr<logger_client>> &clients) {
    acceptor.async_accept(
            [&](beast::error_code ec, tcp::socket socket) {
                if (!ec)
                    std::make_shared<http_connection_client>(std::move(socket), message_map_, clients)->start();
                http_server(acceptor, message_map_, clients);
            });
}
