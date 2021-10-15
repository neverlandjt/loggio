//
// Created by ytymchenko on 14.10.2021.
//

#ifndef LOGGIO_LOGGER_CLIENT_H
#define LOGGIO_LOGGER_CLIENT_H
#include <grpcpp/grpcpp.h>
#include "logger.grpc.pb.h"
#include <memory>

using loggio::Logger;
using loggio::LoggerRequest;
using loggio::LoggerReply;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class logger_client  {
public:
    explicit logger_client(std::shared_ptr<Channel> channel)
            : stub_(Logger::NewStub(channel)) {}

    std::string UpdateList(const std::string& user);


private:
    std::unique_ptr<Logger::Stub> stub_;
};


#endif //LOGGIO_LOGGER_CLIENT_H
