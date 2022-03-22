#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <chrono>

#include "include/RDF_index.h"


RDF_index::RDF_index(){
    this->table = new std::vector<int*>;
    this->table->resize(16384);
    this->Is = new std::vector<int>;
    this->Is->resize(16384);
    this->Ip = new std::vector<int>;
    this->Ip->resize(16384);
    this->Io = new std::vector<int>;
    this->Io->resize(16384);            

    this->Isp  = new map_t();
    this->Iop  = new map_t();
    this->Ispo = new map_t();
    this->size_Io = 0;
    this->size_Is = 0;
    this->num_element = 0;
}

int RDF_index::size_of_table(){
    return this->num_element;
}

void RDF_index::resize(){
    int current_size = this->table->size();
    this->table->resize(current_size << 1);
    this->Is->resize(current_size << 1);    
    this->Ip->resize(current_size << 1);
    this->Io->resize(current_size << 1);
}

inline void RDF_index::update_Isp(struct Triple t){
    t.o = 0;
    if((*this->Is)[t.s] == 0){
        (*this->Is)[t.s] = this->num_element;
        (*this->Isp)[t] = this->num_element;
        ++this->size_Is;
    }
    else if(this->Isp->find(t) != this->Isp->end()){
        if((*this->table)[(*this->Isp)[t] - 1][Nsp] != EndOfNode)
            (*this->table)[this->num_element - 1][Nsp] = (*this->table)[(*this->Isp)[t] - 1][Nsp];
        (*this->table)[(*this->Isp)[t] - 1][Nsp] = this->num_element - 1;
    }
    else{
        (*this->Isp)[t] = this->num_element;
        (*this->table)[this->num_element - 1][Nsp] = (*this->Is)[t.s] - 1;
        (*this->Is)[t.s] = this->num_element;
    }
}

inline void RDF_index::update_Iop(struct Triple t){
    t.s = 0;
    if((*this->Io)[t.o] == 0){
        (*this->Io)[t.o] = this->num_element;
        (*this->Iop)[t] = this->num_element;
        ++this->size_Io;
    }
    else if(this->Iop->find(t) != this->Iop->end()){
        if((*this->table)[(*this->Iop)[t] - 1][Nop] != EndOfNode)
            (*this->table)[this->num_element - 1][Nop] = (*this->table)[(*this->Iop)[t] - 1][Nop];
        (*this->table)[(*this->Iop)[t] - 1][Nop] = this->num_element - 1;
    }
    else{
        (*this->Iop)[t] = this->num_element;
        (*this->table)[this->num_element - 1][Nop] = (*this->Io)[t.o] - 1;
        (*this->Io)[t.o] = this->num_element;
    }
}

inline void RDF_index::update_Ip(struct Triple t){
    if((*this->Ip)[t.p] == 0){
        (*this->Ip)[t.p] = this->num_element;
    }
    else{
        if((*this->table)[(*this->Ip)[t.p] - 1][Np] != EndOfNode)
            (*this->table)[this->num_element - 1][Np] = (*this->table)[(*this->Ip)[t.p] - 1][Np];
        (*this->table)[(*this->Ip)[t.p] - 1][Np] = this->num_element - 1;
    }
}

void RDF_index::add(struct Triple t){
    int* T_new = new int[6];
    T_new[0] = t.s; T_new[1] = t.p; T_new[2] = t.o;
    T_new[3] = -1;  T_new[4] = -1;  T_new[5] = -1;
    
    if(this->Ispo->find(t) != this->Ispo->end())
        return;

    if(this->table->size() < (this->num_element + 2))
        resize();

    (*this->table)[this->num_element] = T_new;

    ++this->num_element;

    update_Isp(t);
    update_Iop(t);
    update_Ip(t);

    this->Ispo->emplace(t, this->num_element);

}

void RDF_index::print_map(){
    for(auto kv : *this->Isp){
        std::cout << kv.first.s << " " << kv.first.p << " " << kv.first.o << ": " << kv.second << std::endl;
    }
}
#include <fstream>

void RDF_index::print_table(){

  if(true){
    std::ofstream MyFile("filename.txt");
        for(auto kv : *this->Ispo){
            MyFile << kv.first.s << " " << kv.first.p << " " << kv.first.o << " " << kv.second << '\n';

        }
  MyFile.close();

    }

    if(false){
        std::ofstream MyFile("filename2.txt");

            for (int i = 0; i < this->Is->size(); i++){
                MyFile << i << ": " << this->Is->at(i)  << " ";

            MyFile << "\n";
            }
  MyFile.close();

    }

}


inline void RDF_index::evaluate_SYZ(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            if((*this->Is)[t.s] == 0){
                index = EndSearch;
                return;
            }
            COPY(result, index, (*this->table)[(*this->Is)[t.s] - 1], Nsp);
        }
        else{
            COPY(result, index, (*this->table)[index], Nsp);
        }
    } while (t.p == t.o && result.p != result.o && index > EndOfNode);
    if(t.p == t.o && result.p != result.o)
        index = EndSearch;
}

inline void RDF_index::evaluate_SPZ(struct Triple t, struct Triple& result, int& index){
    t.o = 0;
    if((*this->Isp)[t] == 0){
        index = EndSearch;
        return;
    }
    if(index == FirstSearch){
        COPY(result, index, (*this->table)[(*this->Isp)[t] - 1], Nsp);
    }
    else{
        COPY(result, index, (*this->table)[index], Nsp);
    }
    if(result.s != t.s || result.p != t.p)
        index = EndSearch;
}

inline void RDF_index::evaluate_XPO(struct Triple t, struct Triple& result, int& index){
    t.s = 0;
    if((*this->Iop)[t] == 0){
        index = EndSearch;
        return;
    }
    if(index == FirstSearch){
        COPY(result, index, (*this->table)[(*this->Iop)[t] - 1], Nop);
    }
    else{
        COPY(result, index, (*this->table)[index], Nop);
    }
    if(result.o != t.o || result.p != t.p)
        index = EndSearch;
}

inline void RDF_index::evaluate_SPO(struct Triple t, struct Triple& result, int& index){
    if((*this->Ispo)[t] != 0){
        COPY_TRIPLE(result, t);
        index = EndOfNode;
    }
    else{
        index = EndSearch;
    }
}

inline void RDF_index::evaluate_XYO(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            if((*this->Io)[t.o] == 0){
                index = EndSearch;
                return;
            }
            COPY(result, index, (*this->table)[(*this->Io)[t.o] - 1], Nop);
        }
        else{
            COPY(result, index, (*this->table)[index], Nop);
        }
    } while (t.p == t.s && result.p != result.s && index > EndOfNode);

    if(t.p == t.s && result.p != result.s)
        index = EndSearch;
}

inline void RDF_index::evaluate_SYO(struct Triple t, struct Triple& result, int& index){
   if(this->size_Is < this->size_Io){
        do{
            if(index == FirstSearch){
                if((*this->Is)[t.s] == 0){
                    index = EndSearch;
                    return;
                }
                COPY(result, index, (*this->table)[(*this->Is)[t.s] - 1], Nsp);
            }
            else{
                COPY(result, index, (*this->table)[index], Nsp);       
            }
        } while(t.o != result.o && index > EndOfNode);
        if(t.o != result.o)
            index = EndSearch;
    }
    else{
        do{
            if(index == FirstSearch){
                if((*this->Io)[t.o] == 0){
                    index = EndSearch;
                    return;
                }
                COPY(result, index, (*this->table)[(*this->Io)[t.o] - 1], Nop);
            }
            else{
                COPY(result, index, (*this->table)[index], Nop);
            }
        } while(t.s != result.s && index > EndOfNode);
        if(t.s != result.s)
            index = EndSearch;
    }
}

inline void RDF_index::evaluate_XPZ(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            if((*this->Ip)[t.p] == 0){
                index = EndSearch;
                return;
            }
            COPY(result, index, (*this->table)[(*this->Ip)[t.p] - 1], Np);
        }
        else{
            COPY(result, index, (*this->table)[index], Np);
        }
    } while (t.s == t.o && result.s != result.o && index > EndOfNode);
    if(t.s == t.o && result.s != result.o)
        index = EndSearch;       
}

inline void RDF_index::evaluate_SXX(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)
        index = 1;
    if(sub_index == EndOfNode){
        ++index;
        sub_index = FirstSearch;
    }
    for(; index < this->Is->size(); index++){
        if((*this->Is)[index] > 0){
            evaluate_SYZ({index, X, X}, result, sub_index);
            if(sub_index != EndSearch)
                return;
        }
        sub_index = FirstSearch;
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XXO(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)
        index = 1;
    if(sub_index == EndOfNode){
        ++index;
        sub_index = FirstSearch;
    }
    for(; index < this->Io->size(); index++){
        if((*this->Io)[index] > 0){
            evaluate_XYO({X, X, index}, result, sub_index);
            if(sub_index != EndSearch)
                return;
        }
        sub_index = FirstSearch;
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XPX(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)
        index = 1;
    if(sub_index == EndOfNode){
        ++index;
        sub_index = FirstSearch;
    }

    for(; index < this->Ip->size(); index++){
        if((*this->Ip)[index] > 0){
            evaluate_XPZ({X, index, X}, result, sub_index);
            if(sub_index != EndSearch)
                return;
        }
        sub_index = FirstSearch;
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XXX(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)
        index = 1;

    for(; index < this->Is->size(); index++){
        if((*this->Is)[index] > 0){
            evaluate_SPO({index, index, index}, result, sub_index);
            if(sub_index == EndOfNode){
                ++index;
                return;
            }
        }
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XYZ(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch){
        COPY_TABLE(result, (*this->table)[0]);
        index = 1;
    }
    else{
        COPY_TABLE(result, (*this->table)[index]);
        ++index;
    }   
    if(index >= this->num_element)
        index = EndOfNode;     
}

void RDF_index::evaluate(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(t.s >= 0 && t.p >= 0 & t.o >= 0){ // SPO
        evaluate_SPO(t, result, index);
        return;
    }

    if(t.s < 0 && t.p >= 0 && t.o < 0){ // ?XP?Z
        evaluate_XPZ(t, result, index);
        return;
    }

    if(t.s < 0 && t.p >= 0 && t.o >= 0){ // ?XPO
        evaluate_XPO(t, result, index);
        return;
    }

    if(t.s < 0 && t.p < 0 && t.o >= 0){ // ?X?YO
        evaluate_XYO(t, result, index);
        return;
    }

    if(t.s >= 0 && t.p < 0 && t.o < 0){ // S?Y?Z
        evaluate_SYZ(t, result, index);
        return;
    }

    if(t.s >= 0 && t.p < 0 && t.o >= 0){ // S?YO
        evaluate_SYO(t, result, index);
        return;
    }

    if(t.s >= 0 && t.p >= 0 && t.o < 0){ // SP?Z
        evaluate_SPZ(t, result, index);
        return;
    }
    
    if(t.s < 0 && t.p < 0 && t.o < 0){ // ?X?Y?Z
        if(t.s != t.p && t.s != t.o && t.p != t.o){
            evaluate_XYZ(t, result, index, sub_index);    
        }
        else if(t.s == t.p && t.s != t.o && t.p != t.o){
            evaluate_XXO(t, result, index, sub_index);
        }
        else if(t.p == t.o && t.s != t.p && t.s != t.o){
            evaluate_SXX(t, result, index, sub_index);
        }
        else if(t.s == t.o && t.s != t.p && t.o != t.p){
            evaluate_XPX(t, result, index, sub_index);
        }
        else if(t.s == t.p && t.p == t.o && t.o == t.s){
            evaluate_XXX(t, result, index, sub_index);
        }
        return;
    }

}

void RDF_index::print_I(std::vector<int>* vec){
    for(int i = 0; i < vec->size(); i++){
        if((*vec)[i] != 0)
            std::cout << i << ": " << (*vec)[i]-1<< " ";
        else
            std::cout << " " << " ";
    }
    std::cout << std::endl;
}

/*
int main(){
    struct Triple t0 = {1, 2, 2};
    struct Triple t1 = {1, 3, 2};
    struct Triple t2 = {1, 3, 3};
    struct Triple t3 = {1, 3, 4};
    struct Triple t4 = {1, 4, 2};
    struct Triple t5 = {2, 1, 3};
    struct Triple t6 = {1, 1, 1};
    struct Triple t7 = {3, 4, 5};
    struct Triple t8 = {4, 2, 2};
    struct Triple t9 = {5, 2, 2};
    struct Triple t10 = {3, 3, 2};
    struct Triple t11 = {4, 4, 2};
    struct Triple t12 = {1, 6, 6};
    struct Triple t13 = {2, 1, 1};
    struct Triple t14 = {1, 5, 2};
    struct Triple t15 = {1, 7, 2};
    struct Triple t16 = {2, 3, 3};
    struct Triple t17 = {3, 3, 3};
    struct Triple t18 = {3, 4, 3};
    struct Triple t19 = {3, 1, 3};
    struct Triple t20 = {3, 6, 3};
    RDF_index* table = new RDF_index();

    table->add(t0);
    table->add(t1);
    table->add(t2);
    table->add(t3);
    table->add(t4);
    table->add(t5);
    table->add(t6);
    table->add(t7);
    table->add(t8);
    table->add(t9);
    table->add(t10);
    table->add(t11);
    table->add(t12);
    table->add(t13);
    table->add(t14);
    table->add(t15);
    table->add(t16);
    table->add(t17);
    table->add(t18);
    table->add(t19);
    table->add(t20);
    

    table->print_table();
    //table->print_I(table->Ip);
    //table->print_map();
    //table->print_I(table->Is);
    struct Triple t;
    int index = FirstSearch; int sub_index = FirstSearch;
    struct Triple q = {1, 2, Y};
    std::cout << "start search " << std::endl;
    while((index != EndOfNode && index != EndSearch)){
        table->evaluate(q, t, index, sub_index);
        if(index >= EndOfNode)
            std::cout << index << ": " << t.s << t.p << t.o << std::endl;
    }
    
    return 0;
}
*/