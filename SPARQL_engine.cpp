#include "include/SPARQL_engine.h"

SPARQL_engine::SPARQL_engine(){
    this->query = NULL;
}

void SPARQL_engine::join(Query_parser* query){
    this->query = query;
    std::unordered_map<int, int> sigma; 
    this->query->result_size = 0;
    if(this->query->output){
        for(auto kv : *this->query->Voutput){
            std::cout << kv.first << " ";
        } 
        std::cout << std::endl;
    }
    join_helper(sigma, 1);
}

void SPARQL_engine::join_helper(std::unordered_map<int, int> & sigma, int i){
    if(i >= this->query->num_Tps + 1){
        if(this->query->output){
            for(auto kv : *this->query->Voutput){
                std::cout << "<" << this->query->Turtle_handler->idx2IRI->at(sigma[kv.second]) << "> ";
            }
            std::cout << std::endl;
        }
        ++this->query->result_size;
    }
    else{
        struct Triple question;
        struct Triple result;
        int index = FirstSearch; int sub_index = FirstSearch;
        while((index != EndOfNode && index != EndSearch)){
            question = this->query->Tps->at(i-1);
            if(sigma.count(question.s))
                question.s = sigma[question.s];
            if(sigma.count(question.p))
                question.p = sigma[question.p];
            if(sigma.count(question.o))
                question.o = sigma[question.o];
            this->query->Turtle_handler->table->evaluate(question, result, index, sub_index);
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