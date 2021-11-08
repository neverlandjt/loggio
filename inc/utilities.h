//
// Created by ytymchenko on 15.10.2021.
//

#ifndef LOGGIO_UTILITIES_H
#define LOGGIO_UTILITIES_H

#include "logger_client.h"
#include "latch.h"
#include <sstream>
#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

void sync_nodes(logger_client &greeter, const std::string &message, std::shared_ptr<latch> &latch_) {
    std::string reply = greeter.UpdateList(message);
    std::cout << "received: " << reply << std::endl;
    latch_->count_down();
}

ptree parse_request(const std::string &json) {
    ptree pt;
    std::istringstream is(json);
    read_json(is, pt);
    return pt;
}


#endif //LOGGIO_UTILITIES_H
