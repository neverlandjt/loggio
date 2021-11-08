//
// Created by ytymchenko on 15.10.2021.
//

#ifndef LOGGIO_HTTP_CONNECTION_H
#define LOGGIO_HTTP_CONNECTION_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>
#include "concurrent_map.h"

namespace beast = boost::beast;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

class http_connection : public std::enable_shared_from_this<http_connection> {
public:
    explicit http_connection(tcp::socket socket, concurrent_map<std::string>& m)
            : socket_(std::move(socket)), m(m) {
    }

    void start() {
        read_request();
        check_deadline();
    }

protected:
    concurrent_map<std::string>& m;
    tcp::socket socket_;
    beast::flat_buffer buffer_{8192};
    beast::http::request<beast::http::string_body> request_;
    beast::http::response<beast::http::dynamic_body> response_;

    asio::steady_timer deadline_{
            socket_.get_executor(), std::chrono::seconds(60)};

    void
    read_request() {
        beast::http::async_read(
                socket_,
                buffer_,
                request_,
                [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
                    boost::ignore_unused(bytes_transferred);
                    if (!ec) self->process_request();
                });
    }

    virtual void process_request() ;

    void process_get();


    void write_response() ;

    void check_deadline() {
        deadline_.async_wait([self = shared_from_this()](beast::error_code ec) {
            if (!ec) self->socket_.close(ec);

        });
    }
};


#endif //LOGGIO_HTTP_CONNECTION_H
