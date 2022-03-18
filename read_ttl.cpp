#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "include/read_ttl.h"

template<typename K, typename V>
void print_map(std::unordered_map<K, V> const &m){
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

read_ttl::read_ttl(){
    this->table = new Table();
    this->idx2IRI = new std::vector<std::string>();
    this->IRI2idx = new std::unordered_map<std::string, int>();
    this->counter = 0;
}

void read_ttl::load(std::string file_name){
    // Read from the text file
    std::ifstream MyReadFile(file_name);
    int i = 0;
    std::string myText;
    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
        // Output the text from the file
        tokenize(myText);
        i++;
        //break;
    }
    // Close the file
    MyReadFile.close();
}

void read_ttl::tokenize(std::string s) {
    int start = 0;
    int end = s.find(" ");
    struct Triple triple;
    int i = 0;
    while (end != -1) {
        std::string token = s.substr(start, end - start);
        if(((token.compare(0,1,"<") == 0) && (token.compare(token.length()-1,1,">") == 0)) ||
            ((token.compare(0,1,"\"") == 0) && (token.compare(token.length()-1,1,"\"") == 0))){
            token = token.substr(1, token.length()-2);
        }

        if(!IRI2idx->count(token)){
            (*IRI2idx)[token] = counter;
            idx2IRI->push_back(token);
            counter++;
        }
        start = end + 1;
        end = s.find(" ", start);
        if(i == 0){
            triple.s = (*IRI2idx)[token];
        }
        else if(i == 1){
            triple.p = (*IRI2idx)[token];
        }
        else if(i == 2){
            triple.o = (*IRI2idx)[token];
        }
        i++;
    }
    this->table->insert(triple);
}
