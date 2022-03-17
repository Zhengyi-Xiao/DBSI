#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "triple.h"
#include "xxh3.h"
#include "table.h"

#define loading_factor 0.6

class Table{
    
public:

    Table();

    void insert(struct Triple t);
    void resize();
    void update_Ix(std::vector<int>& vec, int key, int value);

    void print_table();
    void print_I(std::vector<int>& vec);

    int size;
    int num_element;
    std::vector<int*>* table;
    std::vector<int> Is, Ip, Io;

};

#endif