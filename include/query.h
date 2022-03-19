#ifndef QUERY_H
#define QUERY_H

#include "RDF_index.h"
#include "Turtle_handler.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <string>

class Query{

public:
    Query(Turtle_handler* Turtle_handler);
    void process(std::string query);
    void join();
    void join_helper(std::unordered_map<int, int>& sigma, int i);
    void set_output(bool output);

    std::map<std::string, int>* Vs, *Voutput;
    std::vector<struct Triple>* Tps;

    int num_Vs, num_Tps;
    Turtle_handler* Turtle_handler;

    int result_size;
    bool output;
};


#endif
    