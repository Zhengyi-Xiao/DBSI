#include <iostream>
#include <vector> 
#include <chrono>

#include "include/RDF_index.h"

/*
int main(){
    struct Triple t0 = {1, 2, 2};
    struct Triple t1 = {1, 3, 2};
    struct Triple t2 = {1, 3, 3};
    struct Triple t3 = {1, 3, 4};    

    map_t* m = new map_t();

    m->emplace(t0, 1);
    m->emplace(t1, 1);
 
    m->emplace(t1, 2);
    m->emplace(t2, 3);
    m->emplace(t3, 4);

    std::cout << (m->find(t1) != m->end()) << std::endl;


    return 0;
}*/