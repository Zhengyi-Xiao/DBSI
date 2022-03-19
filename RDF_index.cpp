#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "include/RDF_index.h"
#include "include/HashTable.h"


RDF_index::RDF_index(){
    this->table = new std::vector<int*>;
    this->Is.resize(1024);
    this->Ip.resize(1024);
    this->Io.resize(1024);
    this->Isp = new HashTable(2);
    this->Iop = new HashTable(2);
    this->Ispo = new HashTable(3);
    this->size_Io = 0;
    this->size_Is = 0;
}

int RDF_index::size_of_table(){
    return this->table->size();
}

void RDF_index::resize(){
    this->Is.resize(this->table->size() << 2);
    this->Ip.resize(this->table->size() << 2);
    this->Io.resize(this->table->size() << 2);
}

void RDF_index::update_Isp(struct Triple t){
    if(this->Is[t.s] == 0){
        this->Is[t.s] = this->table->size();
        this->Isp->insert(t.s, t.p, Negect, this->table->size());
        ++this->size_Is;
    }
    else if(this->Isp->search(t.s, t.p, Negect) != EndOfNode){
        if(this->table->at(this->Isp->search(t.s, t.p, Negect) - 1)[Nsp] == EndOfNode)
            this->table->at(this->Isp->search(t.s, t.p, Negect) - 1)[Nsp] = this->table->size() - 1;
        else{
            this->table->back()[Nsp] = this->table->at(this->Isp->search(t.s, t.p, Negect) - 1)[Nsp];
            this->table->at(this->Isp->search(t.s, t.p, Negect) - 1)[Nsp] = this->table->size() - 1;
        }
    }
    else{
        this->Isp->insert(t.s, t.p, Negect, this->table->size());
        this->table->back()[Nsp] = this->Is[t.s] - 1;
        this->Is[t.s] = this->table->size();
    }
}

void RDF_index::update_Iop(struct Triple t){
    if(this->Io[t.o] == 0){
        this->Io[t.o] = this->table->size();
        this->Iop->insert(t.o, t.p, Negect, this->table->size());
        ++this->size_Io;
    }
    else if(this->Iop->search(t.o, t.p, Negect) != -1){
        if(this->table->at(this->Iop->search(t.o, t.p, -1) - 1)[Nop] == -1)
            this->table->at(this->Iop->search(t.o, t.p, -1) - 1)[Nop] = this->table->size() - 1;
        else{
            this->table->back()[Nop] = this->table->at(this->Iop->search(t.o, t.p, -1) - 1)[Nop];
            this->table->at(this->Iop->search(t.o, t.p, -1) - 1)[Nop] = this->table->size() - 1;
        }
    }
    else{
        this->Iop->insert(t.o, t.p, -1, this->table->size());
        this->table->back()[Nop] = this->Io[t.o] - 1;
        this->Io[t.o] = this->table->size();
    }
}

void RDF_index::update_Ip(struct Triple t){
    if(this->Ip[t.p] == 0){
        this->Ip[t.p] = this->table->size();
    }
    else{
        if(this->table->at(this->Ip[t.p] - 1)[Np] == -1)
            this->table->at(this->Ip[t.p] - 1)[Np] = this->table->size() - 1;
        else{
            this->table->back()[Np] = this->table->at(this->Ip[t.p] - 1)[Np];
            this->table->at(this->Ip[t.p] - 1)[Np] = this->table->size() - 1;
        }
    }
}

void RDF_index::add(struct Triple t){
    int* T_new = new int[6];
    T_new[0] = t.s; T_new[1] = t.p; T_new[2] = t.o;
    T_new[3] = -1; T_new[4] = -1; T_new[5] = -1;
    if(this->Ispo->search(t.s, t.p, t.o) != -1)
        return;

    this->table->push_back(T_new);
    int i = this->table->size();
    if(this->table->size() < this->Is.size())
        this->resize();
    update_Isp(t);
    update_Iop(t);
    update_Ip(t);
    this->Ispo->insert(t.s, t.p, t.o, this->table->size());
}

void RDF_index::print_table(){
    for (int i = 0; i < this->table->size(); i++){
        std::cout << i << ": " << this->table->at(i)[0] << " " << this->table->at(i)[1] << " " << this->table->at(i)[2] << " ";
        for(int j = 3; j < 6; j ++){
            if(this->table->at(i)[j] != -1)
                std::cout << this->table->at(i)[j] << " ";
            else
                std::cout << "  ";
        }
        std::cout << std::endl;
    }
}

inline void RDF_index::evaluate_SYZ(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            COPY(result, index, this->table->at(this->Is[t.s] - 1), Nsp);
        }
        else{
            COPY(result, index, this->table->at(index), Nsp);
        }
    } while (t.p == t.o && result.p != result.o && index > -1);
    if(t.p == t.o && result.p != result.o)
        index = EndSearch;
}

inline void RDF_index::evaluate_SPZ(struct Triple t, struct Triple& result, int& index){
    if(this->Isp->search(t.s, t.p, Negect) <= -1){
        index = EndSearch;
        return;
    }
    if(index == FirstSearch){
        COPY(result, index, this->table->at(this->Isp->search(t.s, t.p, Negect) - 1), Nsp);
    }
    else{
        COPY(result, index, this->table->at(index), Nsp)
    }
    if(result.s != t.s || result.p != t.p)
        index = EndSearch;
}

inline void RDF_index::evaluate_SPO(struct Triple t, struct Triple& result, int& index){
    if(this->Ispo->search(t.s, t.p, t.o) != -1){
        COPY_TRIPLE(result, t);
        index = EndOfNode;
    }
    else{
        index = EndSearch;
    }
}

inline void RDF_index::evaluate_XPO(struct Triple t, struct Triple& result, int& index){
    if(index == FirstSearch){
        COPY(result, index, this->table->at(this->Iop->search(t.o, t.p, Negect) - 1), Nop);
    }
    else{
        COPY(result, index, this->table->at(index), Nop)
    }
    if(result.o != t.o || result.p != t.p)
        index = EndSearch;
}

inline void RDF_index::evaluate_XYO(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            COPY(result, index, this->table->at(this->Io[t.o] - 1), Nop);
        }
        else{
            COPY(result, index, this->table->at(index), Nop);
        }
    } while (t.p == t.s && result.p != result.s && index > -1);
    if(t.p == t.s && result.p != result.s)
        index = EndSearch;
}

inline void RDF_index::evaluate_SYO(struct Triple t, struct Triple& result, int& index){
   if(this->size_Is < this->size_Io){
        do{
            if(index == FirstSearch){
                COPY(result, index, this->table->at(this->Is[t.s] - 1), Nsp);
            }
            else{
                COPY(result, index, this->table->at(index), Nsp);
            }                
        } while(t.o != result.o && index > -1);
        if(t.o != result.o)
            index = EndSearch;
    }
    else{
        do{
            if(index == FirstSearch){
                COPY(result, index, this->table->at(this->Io[t.o] - 1), Nop);
            }
            else{
                COPY(result, index, this->table->at(index), Nop);
            }                
        } while(t.s != result.s && index > -1);
        if(t.s != result.s)
            index = EndSearch;
    }
}

inline void RDF_index::evaluate_XPZ(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            COPY(result, index, this->table->at(this->Ip[t.p] - 1), Np);
        }
        else{
            COPY(result, index, this->table->at(index), Np);
        }
    } while (t.s == t.o && result.s != result.o && index > -1);
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

    for(; index < this->Is.size(); index++){
        if(this->Is[index] > 0){
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

    for(; index < this->Io.size(); index++){
        if(this->Io[index] > 0){
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

    for(; index < this->Ip.size(); index++){
        if(this->Ip[index] > 0){
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

    for(; index < this->Is.size(); index++){
        if(this->Is[index] > 0){
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
        COPY_TABLE(result, this->table->at(0));
        index = 1;
    }
    else{
        COPY_TABLE(result, this->table->at(index));
        ++index;
    }   
    if(index >= this->table->size())
        index = EndOfNode;     
}

void RDF_index::evaluate(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if((t.s >= 0 && this->Is.at(t.s) == 0) || (t.p >= 0 && this->Ip.at(t.p) == 0) || (t.o >= 0 && this->Io.at(t.o) == 0)){
        index = EndSearch;
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

    if(t.s >= 0 && t.p >= 0 & t.o >= 0){ // SPO
        evaluate_SPO(t, result, index);
        return;
    }

}

void RDF_index::print_I(std::vector<int>& vec){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] != 0)
            std::cout << i << ": " << vec[i]-1<< " ";
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

    Table* table = new Table();
    table->add({3,1,3});    
    table->add(t2);
    table->add(t3);
    table->add({2,2,2});
    table->add(t4);
    table->add({1,1,2});
    table->add({3,3,2});  
    table->add({3,3,3});
    table->add({4,4,2});
    table->add(t0);
    table->add(t1);
    table->add(t5);
    table->add(t6);
    table->add(t7);
    table->add({4,4,4});
    table->add(t8);
    table->add(t9);
    table->add({2,1,2});

    table->print_table();

    struct Triple t;
    int index = FirstSearch; int sub_index = FirstSearch;
    struct Triple q = {2, 1, 2};

    std::cout << "start search " << std::endl;
    while((index != EndOfNode && index != EndSearch)){
        table->evaluate(q, t, index, sub_index);
        if(index >= EndOfNode)
            std::cout << index << ": " << t.s << t.p << t.o << std::endl;
    }

    
    return 0;
}*/