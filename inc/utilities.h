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
#include <boost/thread/latch.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

void sync_nodes(logger_client &greeter, LoggerRequest request, std::shared_ptr<latch> &latch_) {
    greeter.pending_q.push_back(q_type{std::move(request), latch_});

}


ptree parse_request(const std::string &json) {
    ptree pt;
    std::istringstream is(json);
    read_json(is, pt);
    return pt;
}


#endif //LOGGIO_UTILITIES_H
