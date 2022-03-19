#ifndef INTERFACE_H
#define INTERFACE_H

#include <map>
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include "query.h"

typedef std::map< std::string, std::function<void(void)> > command_dict;

class Interface{

public:
    Interface();
    void LOAD(std::vector<std::string> token_input);
    void PRINT_TABLE(std::vector<std::string> token_input);
    void SELECT(std::string input);
    void COUNT(std::string input);

    int current_loaded_data;
    read_ttl* ttl;
    Query* query_parser;

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

};


#endif
    