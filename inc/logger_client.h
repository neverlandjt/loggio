//
// Created by ytymchenko on 14.10.2021.
//

#ifndef LOGGIO_LOGGER_CLIENT_H
#define LOGGIO_LOGGER_CLIENT_H

#include <grpcpp/grpcpp.h>
#include "logger.grpc.pb.h"
#include <memory>
#include "concurrent_queue.h"
#include "latch.h"

using loggio::Logger;
using loggio::LoggerRequest;
using loggio::LoggerReply;
using loggio::LoggerHealthCheck;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientWriter;
using grpc::Status;

typedef std::pair<LoggerRequest, std::shared_ptr<latch> > q_type;
typedef concurrent_queue<q_type> Queue;

class logger_client {
public:
    explicit logger_client(std::shared_ptr<Channel> channel, size_t id)
            : stub_(Logger::NewStub(channel)), id(id) {
    }

    [[noreturn]] void UpdateList();

    void HealthCheck();

    size_t id;
    bool healthy= false;
    Queue pending_q;
private:
    std::unique_ptr<Logger::Stub> stub_;
    std::mutex m;
    std::condition_variable cv;
};


#endif //LOGGIO_LOGGER_CLIENT_H
