#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

#include "include/query_parser.h"
#include "include/SPARQL_engine.h"

template<typename K, typename S>
void print_map(std::map<K, S> const &m)
{
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

Query_parser::Query_parser(class Turtle_handler* Turtle_handler){
    this->Turtle_handler = Turtle_handler;
    this->output = true;
}

void Query_parser::set_output(bool output){
    this->output = output;
}

int Query_parser::process(std::string query){
    this->num_Tps = 0;
    this->num_Vs = 0;

    Vs = new std::map<std::string, int>();
    Tps = new std::vector<struct Triple>();
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

/*
int main(){
    //std::string query = "SELECT ?X ?Y WHERE { ?X <hasSon> ?Y . }";
    //std::string query2 = "SELECT ?X ?Y ?Z WHERE { ?X <hasPet> ?Y . ?Y <hasDaughter> ?Z . ?X <marriedTo> ?Z . }";
    Turtle_handler* ttl = new Turtle_handler();
    ttl->load("test.ttl");
    //ttl->table->print_table();

    std::string query = "SELECT ?X ?Z WHERE { ?X <hasAge> ?Y . ?Z <hasAge> ?Y . }";
    //query = "SELECT ?X ?Y WHERE { <Stewie>   <loves> ?Y . ?Y <loves> ?X . }";
    

    
    Query_parser* q = new Query_parser(ttl);
    //q->set_output(false);
    q->process(query);

    print_map(*q->Vs);
    print_map(*q->Voutput);
    
    std::cout << "Triple Patterns are " << std::endl;
    for (auto i: *q->Tps)
        std::cout << i.s << ' ' << i.p << ' ' << i.o << ' ' << std::endl;
    
    struct Triple result;

    struct Triple t;
    int index = FirstSearch; int sub_index = FirstSearch;
    struct Triple q1 = {0, 3, X};

    std::cout << "start search " << std::endl;
    while((index != EndOfNode && index != EndSearch)){
        ttl->table->evaluate(q1, t, index, sub_index);
        if(index >= EndOfNode)
            std::cout << index << ": " << t.s << " " << t.p << " " << t.o << std::endl;
    }
    
    SPARQL_engine* query_engine = new SPARQL_engine();
    query_engine->join(q);

    //q->join();
    
    return 0;
}*/
// 4 0 6 34 719 7790 67 7790 208 4 224 15 1 5916

// COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#GraduateStudent>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  <http://www.Department0.University0.edu/GraduateCourse0>          . }
// COUNT ?X ?Y ?Z WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                         <http://swat.cse.lehigh.edu/onto/univ-bench.owl#GraduateStudent>  . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                         <http://swat.cse.lehigh.edu/onto/univ-bench.owl#University>       . ?Z  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                         <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Department>       . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#memberOf>                 ?Z                                                                .    ?Z  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>        ?Y                                                                . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#undergraduateDegreeFrom>  ?Y                                                                .}
// COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Publication>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#publicationAuthor>  <http://www.Department0.University0.edu/AssistantProfessor0>  .}
// COUNT ?X ?Y1 ?Y2 ?Y3 WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>              <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Professor>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#worksFor>      <http://www.Department0.University0.edu>                    . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#name>          ?Y1                                                         . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#emailAddress>  ?Y2                                                         . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#telephone>     ?Y3                                                         . }
// COUNT ?X WHERE {     ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>          <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Person>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#memberOf>  <http://www.Department0.University0.edu>                 .}
// COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                    <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>  .}
// COUNT ?X ?Y WHERE {    ?X                                                            <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student> .    ?Y                                                            <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Course>  . ?X                                                            <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  ?Y                                                       . <http://www.Department0.University0.edu/AssociateProfessor0>  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#teacherOf>    ?Y                                                       .}
// COUNT ?X ?Y ?Z WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>     . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Department>  .    ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#memberOf>           ?Y                                                           . ?Y  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>  <http://www.University0.edu>                                 . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#emailAddress>       ?Z                                                           .}
// COUNT ?X ?Y ?Z WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>  . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Faculty>  . ?Z  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Course>   . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#advisor>      ?Y                                                        . ?Y  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#teacherOf>    ?Z                                                        .    ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  ?Z                                                        .}
// COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  <http://www.Department0.University0.edu/GraduateCourse0>  . }
// COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#ResearchGroup>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>  <http://www.University0.edu>                                    .}
// COUNT ?X ?Y WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Chair>       . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Department>  .     ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#worksFor>           ?Y                                                           . ?Y  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>  <http://www.University0.edu>                                 .}
// COUNT ?X WHERE { ?X                            <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>            <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Person>  .     <http://www.University0.edu>  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#hasAlumnus>  ?X                                                       .}
// COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#UndergraduateStudent>  .}