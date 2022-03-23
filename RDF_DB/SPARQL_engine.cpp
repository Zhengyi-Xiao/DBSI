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
                    std::cout << "<" << this->query->Turtle_handler->idx2IRI->at(sigma[kv.second]) << ">\t";
                else
                    std::cout << "\"" << this->query->Turtle_handler->idx2IRI->at(sigma[kv.second]) << "\"\t";
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