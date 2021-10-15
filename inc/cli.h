//
// Created by ytymchenko on 15.10.2021.
//

#ifndef LOGGIO_CLI_H
#define LOGGIO_CLI_H


#include <string>
#include <vector>

class cli {
public:
    cli(int argc, char *argv[]);
    unsigned short get_port();
    std::string& get_address();
    std::vector<std::string>& get_addresses();
private:
    unsigned short port;
    std::vector<std::string > addresses;
};


#endif //LOGGIO_CLI_H
