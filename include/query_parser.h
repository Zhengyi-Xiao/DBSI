#ifndef QUERY_H
#define QUERY_H

#include "RDF_index.h"
#include "Turtle_handler.h"


#include <iostream>
#include <unordered_map>
#include <map>
#include <vector>
#include <string>

class Query_parser{

public:
    Query_parser(Turtle_handler* turtle_handler);
    int process(std::string query);
    void set_output(bool output);

    std::map<std::string, int>* Vs, *Voutput;
    std::vector<struct Triple>* Tps;

    int num_Vs, num_Tps;
    Turtle_handler* turtle_handler;

    int result_size;
    bool output;
};


#endif
    