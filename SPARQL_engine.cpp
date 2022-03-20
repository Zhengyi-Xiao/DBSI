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
            bool s = true; bool p = true; bool o = true;
            question = this->query->Tps->at(i-1);
            if(sigma.count(question.s)){
                question.s = sigma[question.s];
                s = false;
            }
            if(sigma.count(question.p)){
                question.p = sigma[question.p];
                p = false;
            }
            if(sigma.count(question.o)){
                question.o = sigma[question.o];
                o = false;
            }
                
            this->query->Turtle_handler->table->evaluate(question, result, index, sub_index);
            
            if(index >= EndOfNode){
                if(s)
                    sigma[question.s] = result.s;
                if(p)
                    sigma[question.p] = result.p;
                if(o)
                    sigma[question.o] = result.o;
                join_helper(sigma, i + 1);
                if(s)
                    sigma.erase(question.s);
                if(p)
                    sigma.erase(question.p);
                if(o)
                    sigma.erase(question.o);
            }
        }        
    }
}