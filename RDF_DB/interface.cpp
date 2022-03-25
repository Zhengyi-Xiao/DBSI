#include "../include/interface.h"

Interface::Interface(){
    this->ttl = new Turtle_handler();
    this->query_parser = new Query_parser(this->ttl);
    this->query_planner = new Query_planner(this->ttl);
    this->query_engine = new SPARQL_engine();
    this->current_loaded_data = 0;
}

void Interface::LOAD(std::vector<std::string> token_input){
    if(token_input.size() < 2){
        std::cout << "Missing the file name!" << std::endl;
    }
    else{
        this->begin = std::chrono::high_resolution_clock::now();
        if(this->ttl->load(token_input[1]) == SUCCES){
            this->end = std::chrono::high_resolution_clock::now();
            std::cout << ((this->ttl->table->size_of_table() - this->current_loaded_data) > 0 ? (this->ttl->table->size_of_table() - this->current_loaded_data)  : 0) << " triples loaded in " << ((float)std::chrono::duration_cast<std::chrono::microseconds>(this->end - this->begin).count()/(float)1000) << " ms." << std::endl;
            this->current_loaded_data += this->ttl->table->size_of_table();
        }
        else{
            std::cout << "Open the file, " << token_input[1] << ", failed." << std::endl;
        }
    }
}

void Interface::SELECT(std::string input){
    std::cout << "----------" << std::endl;
    this->begin = std::chrono::high_resolution_clock::now();
    
    this->query_parser->set_output(true);
    this->query_parser->result_size = 0;
    if(this->query_parser->process(input) == SUCCES){
        std::vector<struct Triple> new_plan;
        this->query_planner->plan_query(*this->query_parser->Tps, new_plan);
        this->query_parser->Tps = &new_plan;
        this->query_engine->join(this->query_parser);
    }
    else{
        std::cout << "FAIL" << std::endl;
        return;
    }
        
    this->end = std::chrono::high_resolution_clock::now();

    std::cout << "----------" << std::endl;
    std::cout << this->query_parser->result_size << " results returned in " << ((float)std::chrono::duration_cast<std::chrono::microseconds>(this->end - this->begin).count()/(float)1000) << " ms." << std::endl;
}

void Interface::COUNT(std::string input){

    this->begin = std::chrono::high_resolution_clock::now();
    this->query_parser->set_output(false);
    this->query_parser->result_size = 0;
    if(this->query_parser->process(input) == SUCCES){
        std::vector<struct Triple> new_plan;
        this->query_planner->plan_query(*this->query_parser->Tps, new_plan);
        this->query_parser->Tps = &new_plan;
        this->query_engine->join(query_parser);
    }
    this->end = std::chrono::high_resolution_clock::now();

    std::cout << this->query_parser->result_size << " results returned in " << ((float)std::chrono::duration_cast<std::chrono::microseconds>(this->end - this->begin).count()/(float)1000) << " ms." << std::endl;
}


Interface::~Interface(){
    delete ttl;
    delete query_engine;
    delete query_parser;
    delete query_planner;
    current_loaded_data = 0;
}