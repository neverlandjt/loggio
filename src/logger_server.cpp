//
// Created by ytymchenko on 14.10.2021.
//

#include "../inc/logger_server.h"

size_t logger_server::rand() {
    std::unique_ptr<Range> rng2 = std::make_unique<Range>();
    rng2->seed(time(NULL));
    boost::normal_distribution<> distribution(100, 5000);
    boost::variate_generator<Range, boost::normal_distribution<> > generator(*rng2, distribution);
    return generator();
}

Status logger_server::UpdateList(ServerContext *context, const LoggerRequest *request, LoggerReply *reply) {
    auto rnd = rand();
    std::this_thread::sleep_for(std::chrono::milliseconds(rnd));
    map.push_pos(request->message(), request->idx());
    reply->set_id(0);

    if (rnd % 2 == 0) {
        std::cout << "Generated error for id [" << request->idx() << "]" << std::endl;
        return grpc::Status(grpc::StatusCode::UNAVAILABLE, "Randomly generated error");
    }

    return Status::OK;
}
