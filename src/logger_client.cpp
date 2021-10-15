//
// Created by ytymchenko on 14.10.2021.
//

#include <logger_client.h>

std::string logger_client::UpdateList(const std::string &user) {
    LoggerRequest request;
    request.set_message(user);

    LoggerReply reply;

    ClientContext context;

    Status status = stub_->UpdateList(&context, request, &reply);

    if (status.ok()) {
        return reply.status();
    } else {
        std::cout << status.error_code() << ": " << status.error_message()
                  << std::endl;
        return "RPC failed";
    }
}


