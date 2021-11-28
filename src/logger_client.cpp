//
// Created by ytymchenko on 14.10.2021.
//

#include <logger_client.h>

[[noreturn]] void logger_client::UpdateList() {


    while (true) {
        LoggerReply reply;

        ClientContext context;
        std::unique_lock<std::mutex> lck(m);
        cv.wait(lck, [this] { return healthy; });
        auto queue_item = pending_q.pop_front();
        Status status = stub_->UpdateList(&context, queue_item.first, &reply);
        if (status.ok()) {
            std::cout << "Node_" << id << ": " << "Id ["<< queue_item.first.idx()<<"] - acknowledged" << std::endl;
            queue_item.second->count_down();
        } else {
            std::cout << "Node_" << id << ": " << status.error_message() <<". Id ["<<queue_item.first.idx()<<"] - requeued" << std::endl;
            pending_q.push_back(std::move(queue_item));
        }
    }
}

void logger_client::HealthCheck() {
    LoggerHealthCheck request;
    request.set_healthy(true);

    LoggerHealthCheck reply;

    ClientContext context;

    Status status = stub_->HealthCheck(&context, request, &reply);

    if (status.ok()) {
        std::cout << "Node_" << id << ": healthy" << std::endl;
        healthy = reply.healthy();
        cv.notify_one();
    } else {
        std::cout << "Node_" << id << ": " << status.error_message() << std::endl;
        healthy = false;
    }
}


