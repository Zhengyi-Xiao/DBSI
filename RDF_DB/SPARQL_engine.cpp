#include "../include/SPARQL_engine.h"

SPARQL_engine::SPARQL_engine(){
    this->query = NULL;
}

// Join starts here
void SPARQL_engine::join(Query_parser* query){
    this->query = query;
    this->query->result_size = 0;

    if(this->query->output){
        for(auto kv : *this->query->Voutput){
            std::cout << kv.first << "\t";
        } 
        std::cout << std::endl;
    }
    std::unordered_map<int, int> sigma; 
    join_helper(sigma, 1);
}

// This algorithm is implemented based on the model answer.
void SPARQL_engine::join_helper(std::unordered_map<int, int> & sigma, int i){
    if(i >= this->query->num_Tps + 1){
        if(this->query->output){
            for(auto kv : *this->query->Voutput){
                std::string output = this->query->Turtle_handler->idx2IRI->at(sigma[kv.second]);
                if(output.size() > 5 and output.substr(0,4) == "http") // to see if it is a IRI
                    std::cout << "<" << this->query->Turtle_handler->idx2IRI->at(sigma[kv.second]) << "> \t";
                else
                    std::cout << "\"" << this->query->Turtle_handler->idx2IRI->at(sigma[kv.second]) << "\" \t";
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
            question.s = (question.s < 0 && sigma[question.s] > 0) ? sigma[question.s] : question.s;
            question.p = (question.p < 0 && sigma[question.p] > 0) ? sigma[question.p] : question.p;
            question.o = (question.o < 0 && sigma[question.o] > 0) ? sigma[question.o] : question.o;

            this->query->Turtle_handler->table->evaluate(question, result, index, sub_index);
            
            if(index >= EndOfNode){
                sigma[question.s] = result.s;                
                sigma[question.p] = result.p;
                sigma[question.o] = result.o;

                join_helper(sigma, i + 1);

                if(question.s < 0)
                    sigma[question.s] = 0;
                if(question.p < 0)
                    sigma[question.p] = 0;
                if(question.o < 0)
                    sigma[question.o] = 0;
            }
        }        
    }
}