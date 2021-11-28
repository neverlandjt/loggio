//
// Created by ytymchenko on 14.10.2021.
//

#ifndef LOGGIO_LOGGER_SERVER_H
#define LOGGIO_LOGGER_SERVER_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <vector>
#include <string>

#include "boost/random.hpp"
#include <boost/random/uniform_int_distribution.hpp>

#include <chrono>
#include <thread>

#include "concurrent_map.h"


typedef boost::mt19937 Range;

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else

#include "logger.grpc.pb.h"

#endif
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;
using loggio::Logger;
using loggio::LoggerReply;
using loggio::LoggerRequest;
using loggio::LoggerHealthCheck;


class logger_server final : public Logger::Service {

    static size_t rand();

    Status UpdateList(ServerContext *context, const LoggerRequest *request,
                      LoggerReply *reply) override;

    Status HealthCheck(ServerContext *context, const LoggerHealthCheck *request,
                       LoggerHealthCheck *reply) override {
        reply->set_healthy(true);
        return Status::OK;
    }

public:
    concurrent_map<std::string> map;

};


#endif //LOGGIO_LOGGER_SERVER_H
