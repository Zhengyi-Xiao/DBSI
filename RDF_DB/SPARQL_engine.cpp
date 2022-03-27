#include "../include/SPARQL_engine.h"

SPARQL_engine::SPARQL_engine(){
    this->query = NULL;
}

// Join starts here
void SPARQL_engine::join(Query_parser* query){
    this->query = query;
    this->query->result_size = 0;

    // print the header (variable names) for join
    if(this->query->output){
        for(auto kv : *this->query->Voutput){
            std::cout << kv.first << "    ";
        } 
        std::cout << std::endl;
    }

    std::vector<int> sigma; 
    sigma.resize(this->query->num_Vs << 1);
    
    join_helper(sigma, 1);
}

// This algorithm is implemented based on the model answer.
void SPARQL_engine::join_helper(std::vector<int> & sigma, int i){
    if(i >= this->query->num_Tps + 1){
        if(this->query->output){ // print output
            for(auto kv : *this->query->Voutput){
                std::string output = this->query->Turtle_handler->idx2IRI->at(sigma[-kv.second]);
                if(output.size() > 5 and output.substr(0,4) == "http") // to see if it is a IRI
                    std::cout << "<" << this->query->Turtle_handler->idx2IRI->at(sigma[-kv.second]) << ">    ";
                else
                    std::cout << "\"" << this->query->Turtle_handler->idx2IRI->at(sigma[-kv.second]) << "\"    ";
            }
            std::cout << std::endl;
        }
        ++this->query->result_size;
    }
    else{
        struct Triple question; struct Triple result;
        int index = FirstSearch; int sub_index = FirstSearch;

        while((index != EndOfNode && index != EndSearch)){
            question = this->query->Tps->at(i - 1);
            // replace already known variables by its values.
            if(question.s < 0 && sigma[-question.s] > 0) question.s = sigma[-question.s];
            if(question.p < 0 && sigma[-question.p] > 0) question.p = sigma[-question.p];
            if(question.o < 0 && sigma[-question.o] > 0) question.o = sigma[-question.o];

            // evaluate
            this->query->Turtle_handler->table->evaluate(question, result, index, sub_index);
            
            // as long as it is not an illegal search, we keep doing this.
            if(index >= EndOfNode){ 
                // replace the just known variables by values
                if(question.s < 0) sigma[-question.s] = result.s;   
                if(question.p < 0) sigma[-question.p] = result.p;
                if(question.o < 0) sigma[-question.o] = result.o;
                
                join_helper(sigma, i + 1);
                
                // delete the just known varaibles
                if(question.s < 0) sigma[-question.s] = 0;
                if(question.p < 0) sigma[-question.p] = 0;
                if(question.o < 0) sigma[-question.o] = 0;
            }
        }        
    }
}