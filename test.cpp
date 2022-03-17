#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "include/triple.h"
#include "include/table.h"

Table::Table(){
    this->table = new std::vector<int*>;
    this->num_element = 0;
    this->table->resize(16);
    this->size = 2;
    this->Is.resize(8);
    this->Ip.resize(8);
    this->Io.resize(8);
}

void Table::resize(){
    std::vector<int*>* copy = new std::vector<int*>;
    int new_size = this->table->size() << 1;
    copy->resize(new_size);
    char concatenated[12];
    
    for(int j = 0; j < this->size; j++){
        if(this->table->at(j) != NULL){
            std::memcpy(concatenated, (char*)&this->table->at(j)[0], sizeof(int));
            std::memcpy(concatenated+4, (char*)&this->table->at(j)[1], sizeof(int));
            std::memcpy(concatenated+8, (char*)&this->table->at(j)[2], sizeof(int));    
            int i = XXH32(concatenated, 12, 0) % new_size;
            int* T = copy->at(i);

            while(T != NULL){
                i = (i + 1) % new_size;
                T = copy->at(i);
            }
            copy->at(i) = this->table->at(j);      
        }
    }
    this->size = new_size;
    delete this->table;
    this->table = copy;
}

void Table::update_Ix(std::vector<int>& vec, int key, int value){
    if(key > vec.size()){
        vec.resize(vec.size()<<1);
    }
    if(vec[key] == 0){
        vec[key] = value;
    }
}

void Table::insert(struct Triple t){
    char concatenated[12];
    std::memcpy(concatenated, (char*)&t.s, sizeof(int));
    std::memcpy(concatenated+4, (char*)&t.p, sizeof(int));
    std::memcpy(concatenated+8, (char*)&t.o, sizeof(int));    
    int i = XXH32(concatenated, 12, 0) % this->size;

    if(this->num_element/this->size > loading_factor)
        this->resize();

    int* T = this->table->at(i);
    while(T != NULL){
        if((T[0] == t.s) && (T[1] == t.p) && (T[2] == t.o)){
            return;
        }
        i = (i + 1) % this->size;
        T = this->table->at(i);
    }

    int* T_new = new int[6];
    T_new[0] = t.s; T_new[1] = t.p; T_new[2] = t.o;
    

    update_Ix(this->Is, t.s, i+1);
    update_Ix(this->Ip, t.p, i+1);
    update_Ix(this->Io, t.o, i+1);

    this->table->at(i) = T_new;
    ++this->num_element;
}

void Table::print_table(){
    for (int i = 0; i < this->size; i++){
        if(this->table->at(i) != NULL)
            std::cout << i << ": " << this->table->at(i)[0] << " " << this->table->at(i)[1] << " " << this->table->at(i)[2] << std::endl;
    }
}

void Table::print_I(std::vector<int>& vec){
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] != 0)
            std::cout << vec[i]-1 << " ";
        else
            std::cout << " " << " ";
    }
    std::cout << std::endl;
}


int main(){
    struct Triple t1 = {1, 3, 2};
    struct Triple t2 = {2, 1, 4};
    struct Triple t3 = {1, 1, 2};
    struct Triple t4 = {1, 3, 4};
    struct Triple t5 = {2, 1, 3};
    struct Triple t6 = {1, 1, 1};

    Table* table = new Table();
    

    table->insert(t1);
    table->insert(t2);
    table->insert(t3);
    table->insert(t4);
    table->insert(t5);
    table->insert(t6);

    table->print_table();
    table->print_I(table->Is);
    table->print_I(table->Ip);
    table->print_I(table->Io);

    return 0;
}