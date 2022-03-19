#include "include/interface.h"

Interface::Interface(){
    this->ttl = new read_ttl();
    this->query_parser = new Query(this->ttl);

    this->begin = std::chrono::steady_clock::now();
    this->end = std::chrono::steady_clock::now();
    this->current_loaded_data = 0;
}

void Interface::LOAD(std::vector<std::string> token_input){
    if(token_input.size() < 2){
        std::cout << "Missing the file name!" << std::endl;
    }
    else{
        this->begin = std::chrono::steady_clock::now();
        if(this->ttl->load(token_input[1]) == SUCCES){
            this->end = std::chrono::steady_clock::now();
            std::cout << this->ttl->table->size_of_table() - this->current_loaded_data << " triples loaded in " << std::chrono::duration_cast<std::chrono::microseconds>(this->end - this->begin).count() << "ms." << std::endl;
            this->current_loaded_data += this->ttl->table->size_of_table();
        }
        else{
            std::cout << "Open the file, " << token_input[1] << ", failed." << std::endl;
        }
    }
}

void Interface::PRINT_TABLE(std::vector<std::string> token_input){
    this->ttl->table->print_table();
}

void Interface::SELECT(std::string input){
    std::cout << "----------" << std::endl;

    this->begin = std::chrono::steady_clock::now();
    this->query_parser->set_output(true);
    this->query_parser->process(input);
    this->query_parser->join();
    this->end = std::chrono::steady_clock::now();

    std::cout << "----------" << std::endl;
    std::cout << this->query_parser->result_size << " results returned in " << std::chrono::duration_cast<std::chrono::microseconds>(this->end - this->begin).count() << " ms." << std::endl;
}

void Interface::COUNT(std::string input){

    this->begin = std::chrono::steady_clock::now();
    this->query_parser->set_output(false);
    this->query_parser->process(input);
    this->query_parser->join();
    this->end = std::chrono::steady_clock::now();

    std::cout << this->query_parser->result_size << " results returned in " << std::chrono::duration_cast<std::chrono::microseconds>(this->end - this->begin).count() << " ms." << std::endl;
}


std::vector<std::string> split(std::string s){
    std::vector<std::string> result;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(" ")) != std::string::npos) {
        token = s.substr(0, pos);
        s.erase(0, pos + 1);
        result.push_back(token);
    }
    result.push_back(s);
    return result;
}

int main() {

    Interface* interface = new Interface();
    std::vector<std::string> token_input;
    std::cout << "> ";
    std::string input;
    while(std::getline(std::cin, input)) { // quit the program with ctrl-d        
        token_input = split(input);


        if(token_input[0] == "QUIT")
            return 0;
        else{
            if(token_input[0] == "LOAD"){
                interface->LOAD(token_input);
            }
            else if(token_input[0] == "PRINT"){
                interface->PRINT_TABLE(token_input);
            }
            else if(token_input[0] == "SELECT"){
                interface->SELECT(input);                
            }
            else if(token_input[0] == "COUNT"){
                interface->COUNT(input);
            }
            else {
                std::cout << "command \"" << input << "\" not known" << std::endl;
            }
        }
        
        std::cout << "> ";
    }
    return 0;
}