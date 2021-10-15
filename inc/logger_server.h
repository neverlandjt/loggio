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

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else
#include "logger.grpc.pb.h"
#endif
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using loggio::Logger;
using loggio::LoggerReply;
using loggio::LoggerRequest;


class logger_server  final : public Logger::Service {

    Status UpdateList(ServerContext* context, const LoggerRequest* request,
                    LoggerReply* reply) override {
        reply->set_status("ok");
        list.emplace_back(request->message());
        return Status::OK;
    }

public:
    std::vector<std::string> list;
};


#endif //LOGGIO_LOGGER_SERVER_H
