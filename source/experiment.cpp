#include "../include/interface.h"
#include <numeric>                                                              

std::string q1 = "COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#GraduateStudent>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  <http://www.Department0.University0.edu/GraduateCourse0>          . }";
std::string q2 = "COUNT ?X ?Y ?Z WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                         <http://swat.cse.lehigh.edu/onto/univ-bench.owl#GraduateStudent>  . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                         <http://swat.cse.lehigh.edu/onto/univ-bench.owl#University>       . ?Z  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                         <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Department>       . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#memberOf>                 ?Z                                                                .    ?Z  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>        ?Y                                                                . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#undergraduateDegreeFrom>  ?Y                                                                .}";
std::string q3 = "COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Publication>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#publicationAuthor>  <http://www.Department0.University0.edu/AssistantProfessor0>  .}";
std::string q4 = "COUNT ?X ?Y1 ?Y2 ?Y3 WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>              <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Professor>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#worksFor>      <http://www.Department0.University0.edu>                    . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#name>          ?Y1                                                         . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#emailAddress>  ?Y2                                                         . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#telephone>     ?Y3                                                         . }";
std::string q5 = "COUNT ?X WHERE {     ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>          <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Person>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#memberOf>  <http://www.Department0.University0.edu>                 .}";
std::string q6 = "COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                    <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>  .}";
std::string q7 = "COUNT ?X ?Y WHERE {    ?X                                                            <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student> .    ?Y                                                            <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Course>  . ?X                                                            <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  ?Y                                                       . <http://www.Department0.University0.edu/AssociateProfessor0>  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#teacherOf>    ?Y                                                       .}";
std::string q8 = "COUNT ?X ?Y ?Z WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>     . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Department>  .    ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#memberOf>           ?Y                                                           . ?Y  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>  <http://www.University0.edu>                                 . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#emailAddress>       ?Z                                                           .}";
std::string q9 = "COUNT ?X ?Y ?Z WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>  . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Faculty>  . ?Z  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Course>   . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#advisor>      ?Y                                                        . ?Y  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#teacherOf>    ?Z                                                        .    ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  ?Z                                                        .}";
std::string q10 = "COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>             <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Student>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#takesCourse>  <http://www.Department0.University0.edu/GraduateCourse0>  . }";
std::string q11 = "COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#ResearchGroup>  . ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>  <http://www.University0.edu>                                    .}";
std::string q12 = "COUNT ?X ?Y WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Chair>       . ?Y  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>                   <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Department>  .     ?X  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#worksFor>           ?Y                                                           . ?Y  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#subOrganizationOf>  <http://www.University0.edu>                                 .}";
std::string q13 = "COUNT ?X WHERE { ?X                            <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>            <http://swat.cse.lehigh.edu/onto/univ-bench.owl#Person>  .     <http://www.University0.edu>  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#hasAlumnus>  ?X                                                       .}";
std::string q14 = "COUNT ?X WHERE {    ?X  <http://www.w3.org/1999/02/22-rdf-syntax-ns#type>  <http://swat.cse.lehigh.edu/onto/univ-bench.owl#UndergraduateStudent>  .}";

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

void query_test(std::string file_path){
    std::vector<std::string> q_list = {q1, q2, q3, q4, q5, q6, q7, q8, q9, q10, q11, q12, q13, q14};
    std::vector<float> time; time.resize(14);
    std::chrono::high_resolution_clock::time_point begin;
    std::chrono::high_resolution_clock::time_point end; 

    Interface* interface = new Interface();
    interface->LOAD(split("LOAD " + file_path));
    for(int i = 0; i < 10; ++i){
        for(int q_idx = 0; q_idx < 14; ++q_idx){
            begin = std::chrono::high_resolution_clock::now();
            interface->query_parser->set_output(false);
            interface->query_parser->result_size = 0;
            if(interface->query_parser->process(q_list[q_idx]) == SUCCES){
                std::vector<struct Triple> new_plan;
                interface->query_planner->plan_query(*interface->query_parser->Tps, new_plan);
                interface->query_parser->Tps = &new_plan;
                interface->query_engine->join(interface->query_parser);
            }
            end = std::chrono::high_resolution_clock::now();
            time[q_idx] += (float)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/(float)1000;
        }
    }

    for(int q_idx = 0; q_idx < 14; ++q_idx){
        float average = (float)time[q_idx]/(float)10;   
        std::cout << "The query " << (q_idx + 1) << " takes " << average << " ms." << std::endl;    
    }
    delete interface;
}

void load_test(std::string file_path0, std::string file_path1, std::string file_path2){
    float time0 = 0.0; float time1 = 0.0; float time2 = 0.0;
    std::chrono::high_resolution_clock::time_point begin;
    std::chrono::high_resolution_clock::time_point end; 

    for(int i = 0; i < 10; ++i){
        Interface* interface = new Interface();
        begin = std::chrono::high_resolution_clock::now();
        interface->ttl->load(file_path0);
        end = std::chrono::high_resolution_clock::now();
        time0 += (float)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/(float)1000;
        delete interface;

        interface = new Interface();
        begin = std::chrono::high_resolution_clock::now();
        interface->ttl->load(file_path1);
        end = std::chrono::high_resolution_clock::now();
        time1 += (float)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/(float)1000;
        delete interface;

        interface = new Interface();
        begin = std::chrono::high_resolution_clock::now();
        interface->ttl->load(file_path2);
        end = std::chrono::high_resolution_clock::now();
        time2 += (float)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/(float)1000;
        delete interface;
    }

    std::cout << "The dataset " << file_path0 << " takes " <<  (float)time0/(float)10 << " ms." << std::endl;    
    std::cout << "The dataset " << file_path1 << " takes " <<  (float)time1/(float)10 << " ms." << std::endl;    
    std::cout << "The dataset " << file_path2 << " takes " <<  (float)time2/(float)10 << " ms." << std::endl;    

}


int main(){
    
    std::cout << "*** Measure Time Took on Load ***" << std::endl;
    load_test("data/LUBM-001-mat.ttl", "data/LUBM-010-mat.ttl", "data/LUBM-100-mat.ttl");
    
    
    std::cout << "** Measure Time Took on Queries **" << std::endl;
    std::cout << "Run 14 queries on LUBM-001-mat.ttl" << std::endl;
    query_test("data/LUBM-001-mat.ttl");
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Run 14 queries on LUBM-010-mat.ttl" << std::endl;
    query_test("data/LUBM-010-mat.ttl");
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Run 14 queries on LUBM-100-mat.ttl" << std::endl;
    query_test("data/LUBM-100-mat.ttl");
    std::cout << "----------------------------------" << std::endl;
    
    return 0;
}
