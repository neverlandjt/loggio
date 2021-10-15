//
// Created by ytymchenko on 15.10.2021.
//

#include <http_connection.h>

void http_connection::process_request() {
    response_.version(request_.version());
    response_.keep_alive(false);

    switch (request_.method()) {
        case beast::http::verb::get:
            response_.result(beast::http::status::ok);
            response_.set(beast::http::field::server, "Beast");
            process_get();
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

void http_connection::process_get() {
    response_.set(beast::http::field::content_type, "text/html");
    std::stringstream ss;
    for (auto &i: v) ss << i << "\n";
    beast::ostream(response_.body()) << ss.str();
}

void http_connection::write_response() {
    beast::http::async_write(socket_, response_, [self = shared_from_this()](beast::error_code ec, std::size_t) {
        self->socket_.shutdown(tcp::socket::shutdown_send, ec);
        self->deadline_.cancel();
    });
}

