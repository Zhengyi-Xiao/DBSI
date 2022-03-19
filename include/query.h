#ifndef QUERY_H
#define QUERY_H

#include "table.h"
#include "read_ttl.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <string>

class Query{

public:
    Query(read_ttl* read_ttl);
    void process(std::string query);
    void join();
    void join_helper(std::unordered_map<int, int>& sigma, int i);
    void set_output(bool output);

    std::map<std::string, int>* Vs, *Voutput;
    std::vector<struct Triple>* Tps;

    int num_Vs, num_Tps;
    read_ttl* read_ttl;

    int result_size;
    bool output;
};


#endif
    