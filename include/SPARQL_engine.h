#ifndef SPARQL_ENGINE_H
#define SPARQL_ENGINE_H

#include <unordered_map>
#include <map>
#include <vector>
#include <string>

#include "query_parser.h"

class SPARQL_engine{

public:
    SPARQL_engine();
    void join(Query_parser* query);
    void join_helper(std::unordered_map<int, int>& sigma, int i);

    Query_parser* query;
};



#endif