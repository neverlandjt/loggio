//
// Created by ytymchenko on 15.10.2021.
//

#ifndef LOGGIO_UTILITIES_H
#define LOGGIO_UTILITIES_H

#include "logger_client.h"

bool sync_nodes(std::vector<logger_client> &clients, const std::string &message, const std::vector<std::string> &v) {
    size_t success_counter = 0;
    for (auto &greeter: clients) {
        std::string reply = greeter.UpdateList(message);
        std::cout << "received: " << reply << std::endl;
        ++success_counter;
    }
    return success_counter == clients.size();
}



#endif //LOGGIO_UTILITIES_H
