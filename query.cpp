#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "include/query.h"

template<typename K, typename S>
void print_map(std::unordered_map<K, S> const &m)
{
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

Query::Query(class read_ttl* read_ttl){
    this->read_ttl = read_ttl;
    this->output = true;
}

void Query::set_output(bool output){
    this->output = output;
}

void Query::process(std::string query){
    this->num_Tps = 0;
    this->num_Vs = 0;

    Vs = new std::map<std::string, int>();
    Tps = new std::vector<struct Triple>();
    Voutput = new std::map<std::string, int>();
    std::vector<std::string> x = split(query, ' ');
    int i = 0;
    /*
    if(x[0] != "SELECT" && x[0] != "COUNT"){
        std::cout << "No select" << std::endl;
        return;
    }*/
    for(i = 1; i < x.size(); i++){
        if(x[i].substr(0,1) == "?" && (this->Vs->find(x[i]) == this->Vs->end())){
            (*this->Vs)[x[i]] = (-1*(this->num_Vs))-1;
            ++this->num_Vs;
        }
    }
    for(i = 1; i < x.size(); i++){
        if(x[i].substr(0,1) == "?" && (this->Voutput->find(x[i]) == this->Voutput->end())){
            (*this->Voutput)[x[i]] = (*this->Vs)[x[i]];
        }
        if(x[i] == "{")
            break;
    }
    ++i;
    
    for(;i < x.size() - 1;){
        struct Triple tmp;
        if(x[i].substr(0,1) == "?"){
            tmp.s = (*this->Vs)[x[i]];
        }
        else{
            std::string tmp_str = x[i].substr(1, x[i].size()-2);
            tmp.s = this->read_ttl->IRI2idx->at(tmp_str);
        }
        
        if(x[i+1].substr(0,1) == "?"){
            tmp.p = (*this->Vs)[x[i+1]];
        }
        else{
            tmp.p = this->read_ttl->IRI2idx->at(x[i+1].substr(1, x[i+1].size()-2));
        }
        if(x[i+2].substr(0,1) == "?"){
            tmp.o = (*this->Vs)[x[i+2]];
        }
        else{
            tmp.o = this->read_ttl->IRI2idx->at(x[i+2].substr(1, x[i+2].size()-2));
        }        
        ++this->num_Tps;
        this->Tps->push_back(tmp);
        i += 4;
    }
}

void Query::join(){
    std::unordered_map<int, int> sigma; 
    this->result_size = 0;
    if(this->output){
        for(auto kv : *this->Voutput){
            std::cout << kv.first << " ";
        } 
        std::cout << std::endl;
    }
    join_helper(sigma, 1);
}

void Query::join_helper(std::unordered_map<int, int> & sigma, int i){
    if(i >= this->num_Tps + 1){
        if(this->output){
            for(auto kv : *this->Voutput){
                std::cout << "<" << this->read_ttl->idx2IRI->at(sigma[kv.second]) << "> ";
            }
            std::cout << std::endl;
        }
        ++this->result_size;
    }
    else{
        struct Triple question;
        struct Triple result;
        int index = FirstSearch; int sub_index = FirstSearch;
        while((index != EndOfNode && index != EndSearch)){
            question = this->Tps->at(i-1);
            if(sigma.count(question.s))
                question.s = sigma[question.s];
            if(sigma.count(question.p))
                question.p = sigma[question.p];
            if(sigma.count(question.o))
                question.o = sigma[question.o];
            this->read_ttl->table->evaluate(question, result, index, sub_index);
            std::unordered_map<int, int> omega;
            omega.insert(sigma.begin(), sigma.end());    

            if(index >= EndOfNode){
                omega[question.s] = result.s;
                omega[question.p] = result.p;
                omega[question.o] = result.o;
                join_helper(omega, i + 1);
            }
        }        
    }
}

/*
int main(){
    //std::string query = "SELECT ?X ?Y WHERE { ?X <hasSon> ?Y . }";
    //std::string query2 = "SELECT ?X ?Y ?Z WHERE { ?X <hasPet> ?Y . ?Y <hasDaughter> ?Z . ?X <marriedTo> ?Z . }";
    read_ttl* ttl = new read_ttl();
    ttl->load("test.ttl");
    ttl->load("test2.ttl");
    ttl->table->print_table();

    std::string query = "SELECT ?X ?Z WHERE { ?X <hasAge> ?Y . ?Z <hasAge> ?Y . }";
    query = "SELECT ?X ?Y WHERE { <Stewie> <loves> ?Y . ?Y <loves> ?X . }";
    Query* q = new Query(ttl);
    q->process(query);

    print_map(*q->Vs);
    print_map(*q->Voutput);

    for (auto i: *q->Tps)
        std::cout << i.s << ' ' << i.p << ' ' << i.o << ' ' << std::endl;

    struct Triple result;

    struct Triple t;
    int index = FirstSearch; int sub_index = FirstSearch;
    struct Triple q1 = {0, 3, X};

    std::cout << "start search " << std::endl;
    while((index != EndOfNode && index != EndSearch)){
        ttl->table->evaluate(q1, t, index, sub_index);
        if(index >= EndOfNode)
            std::cout << index << ": " << t.s << " " << t.p << " " << t.o << std::endl;
    }

    q->join();

    return 0;
}*/
