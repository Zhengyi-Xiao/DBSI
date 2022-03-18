#ifndef READ_TTL_H
#define READ_TTL_H

#include <string.h>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "table.h"

class read_ttl{

public:
    read_ttl(std::string file_name);
    void tokenize(std::string s);

    std::vector<std::string>* idx2IRI;
    std::unordered_map<std::string, int>* IRI2idx;
    int counter;
    Table* table;
};


#endif
    