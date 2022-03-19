#ifndef READ_TTL_H
#define READ_TTL_H

#include <string.h>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "table.h"

#define FAIL    -1
#define SUCCES  1

class read_ttl{

public:
    read_ttl();
    int load(std::string file_name);
    void tokenize(std::string s);

    std::vector<std::string>* idx2IRI;
    std::unordered_map<std::string, int>* IRI2idx;

    int counter;                // counter keeping the index for the main table
    Table* table;               // the table containing the index table
};


#endif
    