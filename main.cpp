
#include "include/interface.h"

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

    while(std::getline(std::cin, input)){ 
        if(input == ""){
            std::cout << ">";
            continue;
        }
        token_input = split(input);

        if(token_input[0] == "QUIT")
            return 0;
        else{
            if(token_input[0] == "LOAD"){
                interface->LOAD(token_input);
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
