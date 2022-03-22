#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "include/Turtle_handler.h"

template<typename K, typename V>
void print_map(std::unordered_map<K, V> const &m){
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

Turtle_handler::Turtle_handler(){
    this->table   = new RDF_index();
    this->idx2IRI = new std::vector<std::string>();
    this->idx2IRI->resize(1024);
    this->IRI2idx = new std::unordered_map<std::string, int>();
    this->counter = 0;
}

// the function to load the file
// return SUCCESS if succeed, else return FAIL with an error message
int Turtle_handler::load(std::string file_name){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    const char* c_file_name = file_name.c_str();

    fp = fopen(c_file_name, "r");
    if (fp == NULL){
        std::cout << "Unable to open the file" << file_name << std::endl;
        return FAIL;
    }

    std::string s_line;
    std::string token;
    struct Triple triple = {-1, -1, -1};
    int i = 0; 
    while ((read = getline(&line, &len, fp)) != -1) {
        s_line = line;
        int start = 0; int end = 0;
        while(start < read){ // tokenized the line 
            
            if(line[start] == '<' || line[start] == '\"'){
                if(i == 3){
                    // the last triple should be "legal" but rather missing the last 
                    // diameter ".", but due to the definition of a vaild triple, I count this as illegal.
                    std::cout << "There is a triple with a missing diameter . " << std::endl;
                    return FAIL;
                }
                bool find = false;
                end = start + 1;
                while(end < len){
                    if(line[end] == '>' || line[end] == '\"'){
                        find = true;
                        break;
                    }
                    ++end;
                }
                if(!find){
                    std::cout << "There is a resources with invalid syntax (missing > or \"). " << std::endl;
                    return FAIL;
                }
                token = s_line.substr(start+1, end-start-1);
                
                if(!IRI2idx->count(token)){
                    (*IRI2idx)[token] = counter;
                    if(counter > idx2IRI->size() - 1)
                        idx2IRI->resize(idx2IRI->size() << 1);
                    idx2IRI->at(this->counter) = token;
                    ++counter;
                }
                
                if(i == 0)
                    triple.s = (*IRI2idx)[token];
                else if(i == 1)
                    triple.p = (*IRI2idx)[token];
                else if(i == 2)
                    triple.o = (*IRI2idx)[token];
                
                ++i;
                start = end;
            }
            if(line[start] == '.'){
                if((triple.s == -1 || triple.p == -1 || triple.o == -1) &&
                    !((triple.s == -1 && triple.p == -1 && triple.o == -1))){
                    std::cout << "there is a triple missing a subject, predicate, or object." << std::endl;
                    return FAIL;
                }
                this->table->add(triple);
                i = 0;
                triple = {-1, -1, -1};
            }
            ++start;
        }
    }

    fclose(fp);
    if (line)
        free(line);
    return SUCCES;
}


#include <chrono>
/*
int main(){
    Turtle_handler* load_file = new Turtle_handler();
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    load_file->load("LUBM-001-mat.ttl");
    load_file->table->print_table();
    //load_file->table->print_I(load_file->table->Is);
    //load_file->table->Isp->print_I();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms." << std::endl;

    return 0;
}*/