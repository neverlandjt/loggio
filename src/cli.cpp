//
// Created by ytymchenko on 15.10.2021.
//

#include "cli.h"
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

cli::cli(int argc, char **argv) {
    // define options to be shown on --help
    po::options_description basic_options("Options");
    basic_options.add_options()
            ("help", "print help message")
            ("port,p", po::value<uint16_t>(), "specify port number");

    // define hidden options
    po::options_description hidden_options("Hidden options");
    hidden_options.add_options()
            ("address,a", po::value<std::vector<std::string>>(), "addresses to serve");

    po::options_description options;
    options.add(basic_options).add(hidden_options);



    po::variables_map vm;
    auto parsed = po::command_line_parser(argc, argv).options(options).run();
    po::store(parsed, vm);

    // show help message
    if (vm.count("help")) {
        std::cout << basic_options << std::endl;
        exit(0);
    }

    // port is not specified
    if (!vm.count("port")) {
        throw std::invalid_argument("Port to use is not specified\n");
    }

    // directory is not specified
    if (!vm.count("address")) {
        throw std::invalid_argument("Address to serve  is not specified\n");
    }
    port = vm["port"].as<unsigned short>();
    addresses = vm["address"].as<std::vector<std::string>>();
}


unsigned short cli::get_port() {
    return port;
}

std::string &cli::get_address() {
    return std::ref(addresses[0]);
}

std::vector<std::string> &cli::get_addresses() {
    return std::ref(addresses);
}


