#ifndef Turtle_handler_H
#define Turtle_handler_H

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <vector>

#include "RDF_index.h"

#define FAIL    -1
#define SUCCES  1

class Turtle_handler{

public:
    Turtle_handler();
    int load(std::string file_name);

    std::vector<std::string>* idx2IRI;
    std::unordered_map<std::string, int>* IRI2idx;

    int counter;                // counter keeping the index for the main table
    RDF_index* table;               // the table containing the index table
};


#endif
    