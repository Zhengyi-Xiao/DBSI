#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>

#include "triple.h"
#include "xxh3.h"
#include "table.h"

#define loading_factor 0.6

class Table{
    // A hash function used to hash a tuple
    struct hash_tuple {
    
        template <class T1, class T2>
    
        size_t operator()(
            const std::tuple<T1, T2>& x)
            const
        {
            return std::get<0>(x)
                ^ std::get<1>(x);
        }
    };
public:

    Table();

    void resize();
    void insert(struct Triple t);
    void print();
    void update_Ix(std::vector<int>& vec, int key, int value);

    int size;
    int num_element;
    std::vector<int*>* table;
    std::vector<int> Is, Ip, Io;
    std::unordered_map<std::tuple<int,int>, int, hash_tuple> Isp, Iop;

};

#endif