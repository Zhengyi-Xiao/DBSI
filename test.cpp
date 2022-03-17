#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "include/table.h"
#include "include/HashTable.h"
//#include "HashTable.cpp"
#define Nsp         3
#define Np          4
#define Nop         5
#define EndOfNode   -1
#define Negect      -1

Table::Table(){
    this->table = new std::vector<int*>;
    this->Is.resize(4);
    this->Ip.resize(4);
    this->Io.resize(4);
    this->Isp = new HashTable(2);
    this->Iop = new HashTable(2);
    this->Ispo = new HashTable(3);
}

void Table::resize(){
    this->Is.resize(this->table->size() << 2);
    this->Ip.resize(this->table->size() << 2);
    this->Io.resize(this->table->size() << 2);
}

void Table::update_Isp(struct Triple t){
    if(this->Is[t.s] == 0){
        this->Is[t.s] = this->table->size();
        this->Isp->insert(t.s, t.p, Negect, this->table->size());
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

void Table::update_Iop(struct Triple t){
    if(this->Io[t.o] == 0){
        this->Io[t.o] = this->table->size();
        this->Iop->insert(t.o, t.p, Negect, this->table->size());
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

void Table::update_Ip(struct Triple t){
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

void Table::insert(struct Triple t){
    int* T_new = new int[6];
    T_new[0] = t.s; T_new[1] = t.p; T_new[2] = t.o;
    T_new[3] = -1; T_new[4] = -1; T_new[5] = -1;

    this->table->push_back(T_new);
    int i = this->table->size();
    if(this->table->size() < this->Is.size())
        this->resize();
    update_Isp(t);
    update_Iop(t);
    update_Ip(t);
    this->Ispo->insert(t.s, t.p, t.o, this->table->size());
}

void Table::print_table(){
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

void Table::evaluate(struct Triple t, struct Triple& result, int& index){
    if(this->Ispo->search(t.s, t.p, t.o) != -1){
        index = this->Ispo->search(t.s, t.p, t.o);
        result.s = t.s;
        result.p = t.p;
        result.o = t.o;
    }
}

void Table::print_I(std::vector<int>& vec){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] != 0)
            std::cout << i << ": " << vec[i]-1<< " ";
        else
            std::cout << " " << " ";
    }
    std::cout << std::endl;
}

int main(){

    struct Triple t1 = {1, 3, 2};
    struct Triple t2 = {1, 3, 3};
    struct Triple t3 = {1, 3, 4};
    struct Triple t4 = {1, 4, 2};
    struct Triple t5 = {2, 1, 3};
    struct Triple t6 = {1, 1, 1};
    struct Triple t7 = {3, 4, 5};

    Table* table = new Table();

    table->insert(t1);
    table->insert(t2);
    table->insert(t3);
    table->insert(t4);
    table->insert(t5);
    table->insert(t6);
    table->insert(t7);

    table->print_table();
    
    struct Triple t;
    int index;
    table->evaluate(t1, t, index);
    std::cout << index << ": " << t.s << t.p << t.o << std::endl;
    return 0;
}