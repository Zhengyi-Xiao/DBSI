#ifndef INTERFACE_H
#define INTERFACE_H

#include <map>
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

#include "query_parser.h"
#include "query_planner.h"
#include "SPARQL_engine.h"

typedef std::map< std::string, std::function<void(void)> > command_dict;

class Interface{

public:
    Interface();
    void LOAD(std::vector<std::string> token_input);
    void PRINT_TABLE(std::vector<std::string> token_input);
    void SELECT(std::string input);
    void COUNT(std::string input);

    int current_loaded_data;
    Turtle_handler* ttl;
    Query_parser* query_parser;
    Query_planner* query_planner;
    SPARQL_engine* query_engine;

    std::chrono::high_resolution_clock::time_point begin;
    std::chrono::high_resolution_clock::time_point end;

};


#endif
    