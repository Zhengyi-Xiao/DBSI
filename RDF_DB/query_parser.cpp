#include "include/query_parser.h"


Query_parser::Query_parser(class Turtle_handler* Turtle_handler){
    this->Turtle_handler = Turtle_handler;
    this->output = true;
}

void Query_parser::set_output(bool output){
    this->output = output;
}

// There are some robustness in the parser making some illegal query, including checking one does not exist
// is handled in the parser.
int Query_parser::process(std::string query){
    this->num_Tps = 0;
    this->num_Vs = 0;

    Vs      = new std::map<std::string, int>();
    Tps     = new std::vector<struct Triple>();
    Voutput = new std::map<std::string, int>();
    
    int begin = 0; int end = 0;
    int len = query.length();
    bool where_appear = false;
    std::vector<int> triple;
    while(begin < len){
        // Check if WHERE appears
        if(!where_appear && query[begin] == 'W'){
            end = begin + 1;
            while(end < len){
                if(query[end] == ' ')
                    break;
                ++end;
            }
            if(query.substr(begin, end - begin) == "WHERE")
                where_appear = true;
            begin = end + 1;
            continue;
        }
        // Store variables into Output list or Variable list.
        if(query[begin] == '?'){
            end = begin + 1;
            while(end < len){
                if(query[end] == ' '){
                    std::string token = query.substr(begin, end - begin);
                    if(!where_appear)
                        (*Voutput)[token] = (-1*num_Vs)-2;
                    if(Vs->find(token) == Vs->end()){
                        (*Vs)[token] = (-1*num_Vs)-2;
                        ++num_Vs;
                    }
                    if(where_appear)
                        triple.push_back((*Vs)[token]);
                    break;
                }
                ++end;
            }
            begin = end + 1;
            continue;
        }
        // Handle IRIs
        if(query[begin] == '<'){
            end = begin + 1;
            while(end < len){
                if(query[end] == '>'){
                    std::string token = query.substr(begin+1, end - begin - 1);
                    if(this->Turtle_handler->IRI2idx->find(token) == this->Turtle_handler->IRI2idx->end()){
                        std::cout << token << " is not presented in the table." << std::endl;
                        return FAIL;
                    }
                    triple.push_back(this->Turtle_handler->IRI2idx->at(token));
                    break;
                }
                ++end;
            }
            begin = end + 1;
            continue;
        } 
        // Handle Literals
        if(query[begin] == '\"'){
            end = begin + 1;
            while(end < len){
                if(query[end] == '\"'){
                    std::string token = query.substr(begin+1, end - begin - 1);
                    if(this->Turtle_handler->IRI2idx->find(token) == this->Turtle_handler->IRI2idx->end()){
                        std::cout << token << " is not presented in the table." << std::endl;
                        return FAIL;
                    }
                    triple.push_back(this->Turtle_handler->IRI2idx->at(token));
                    break;
                }
                ++end;
            }
            begin = end + 1;
            continue;
        }    
        // Store Triple
        if(query[begin] == '.'){
            if(triple.size() == 3){
                ++num_Tps;
                struct Triple t_new = {triple.at(0), triple.at(1), triple.at(2)};
                Tps->push_back(t_new);
                triple.clear();
            }
            else{
                std::cout << "There is a triple pattern does not have all three patterns but comes with a . right after." << std::endl;
                return FAIL;
            }
        }
        ++begin;
    }

    if(!triple.empty()){
        std::cout << "The last triple pattern does not have . in the end or there is an object missing." << std::endl;
        return FAIL;
    }
        

    if(!where_appear){
        std::cout << "The query does not contain WHERE" << std::endl;
        return FAIL;
    }

    return SUCCES;
}