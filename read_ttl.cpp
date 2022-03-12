#include <iostream>
#include <fstream>
#include "include/LinearHashing.h"
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <err.h>
#include <sys/time.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include "include/xxh3.h"
#include<mach/mach.h>


template<typename K, typename V>
void print_map(std::unordered_map<K, V> const &m)
{
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}
std::unordered_map<int, std::string> idx2IRI;
std::unordered_map<std::string, int> IRI2idx;
int counter = 0;

void tokenize(std::string s) {
    int start = 0;
    int end = s.find(" ");
    while (end != -1) {
        std::string token = s.substr(start, end - start);
        if(((token.compare(0,1,"<") == 0) && (token.compare(token.length()-1,1,">") == 0)) ||
            ((token.compare(0,1,"\"") == 0) && (token.compare(token.length()-1,1,"\"") == 0))){
            token = token.substr(1, token.length()-2);
        }

        if(!IRI2idx.count(token)){
            IRI2idx[token] = counter;
            idx2IRI[counter] = token;
            counter++;
        }
        start = end + 1;
        end = s.find(" ", start);
    }
}

int main() {
    std::string myText;
	Hashmap h(0, 5);

    // Read from the text file
    std::ifstream MyReadFile("LUBM-100-mat.ttl");
    int i = 0;
    // Use a while loop together with the getline() function to read the file line by line
    while (getline (MyReadFile, myText)) {
        // Output the text from the file
        tokenize(myText);
        i++;
        //break;
    }
    std::cout << i << std::endl;
    // Close the file
    MyReadFile.close();

    std::cout << IRI2idx.size() << std::endl;
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (KERN_SUCCESS != task_info(mach_task_self(),
                                  TASK_BASIC_INFO, (task_info_t)&t_info,
                                  &t_info_count))
    {
        return -1;
    }
    printf("%lu\n", t_info.resident_size);
    printf("%lu\n", t_info.virtual_size);
}
