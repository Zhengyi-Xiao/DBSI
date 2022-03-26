#ifndef QUERY_PLANNER_H
#define QUERY_PLANNER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "RDF_index.h"
#include "Turtle_handler.h"

#define XXX 8
#define XPX 7
#define SXX 6
#define XXO 5
#define SPX 3
#define XPO 3
#define SXO 2
#define SPO 1

class Query_planner{
    class Turtle_handler* Turtle_handler;
public:

    Query_planner(class Turtle_handler* Turtle_handler);
    void plan_query(std::vector<struct Triple>& U, std::vector<struct Triple>& P);
    void var(const struct Triple t, std::vector<int>& variables);
    void intersection(std::vector<int> v1, std::vector<int> v2, std::vector<int>& v_intersection);
    void union_sets(std::vector<int> v1, std::vector<int> v2, std::vector<int>& v_union);
    void difference(std::vector<struct Triple>& U, struct Triple t);
    int get_score(struct Triple t, const std::vector<int> B);
    
};


#endif
    