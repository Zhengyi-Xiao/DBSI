#include "include/HashTable.h"
#include "include/xxh3.h"
#include "include/triple.h"
#include <vector>

HashTable::HashTable(){
	capacity = 8;
	num_element = 0;
	table = new std::vector<std::vector<int> >;
	table->resize(capacity);
}

void HashTable::insert(int s, int p, int value){
    char concatenated[8];
    std::memcpy(concatenated, (char*)&s, sizeof(int));
    std::memcpy(concatenated+4, (char*)&p, sizeof(int));
    int i = XXH32(concatenated, 8, 0) % this->capacity;

    if((float)this->num_element/this->capacity > loading_factor)
        resize();

    std::vector<int> T = this->table->at(i);
    while(!T.empty()){
        if(((T.at(0) == s) && (T.at(1) == p)) ||  T.at(2) == i){
            return;
        }
        i = (i + 1) % this->capacity;
        T = this->table->at(i);
    }
	std::vector<int> T_new = {s, p, value};
    this->table->at(i) = T_new;
    ++this->num_element;

}

int HashTable::search(int s, int p){
    char concatenated[8];
    std::memcpy(concatenated, (char*)&s, sizeof(int));
    std::memcpy(concatenated+4, (char*)&p, sizeof(int));
    int i = XXH32(concatenated, 8, 0) % this->capacity;

	if (this->table->at(i).empty()){
		return -1;
	}
	else{
		std::vector<int> T = this->table->at(i);
		while(!T.empty()){
			if((T.at(0) == s) && (T.at(1) == p)){
				return T.at(2);
			}
			i = (i + 1) % this->capacity;
			T = this->table->at(i);
		}
	}
	return -1;
}

void HashTable::resize(){
    std::vector<std::vector<int>>* copy = new std::vector<std::vector<int>>;
    int new_size = this->capacity << 1;
    copy->resize(new_size);
    char concatenated[8];

    for(int j = 0; j < this->capacity; j++){
        if(!this->table->at(j).empty()){
            std::memcpy(concatenated, (char*)&this->table->at(j)[0], sizeof(int));
            std::memcpy(concatenated+4, (char*)&this->table->at(j)[1], sizeof(int));
            int i = XXH32(concatenated, 8, 0) % new_size;
			std::vector<int> T = copy->at(i);
            while(!T.empty()){
                i = (i + 1) % new_size;
                T = copy->at(i);
            }
            copy->at(i) = this->table->at(j);      
        }
    }
    this->capacity = new_size;
    delete this->table;
    this->table = copy;
}

void HashTable::print_I(){
    for(int i = 0; i < this->table->size(); i++){
        if(!this->table->at(i).empty()){
            std::cout << this->table->at(i)[0] << "-" << this->table->at(i)[1] << "-" << this->table->at(i)[2] << " ";
		}
        else
            std::cout << " " << " ";
    }
    std::cout << std::endl;
}

HashTable::~HashTable(){
	delete[] table;
	table = NULL;
}
/*
int main(){
    struct Triple t1 = {1, 3, 2};
    struct Triple t2 = {2, 1, 4};
    struct Triple t3 = {1, 1, 2};
    struct Triple t4 = {1, 3, 4};
    struct Triple t5 = {2, 1, 3};
    struct Triple t6 = {1, 1, 1};

	HashTable* table = new HashTable();
	table->insert(t1.s, t1.p, 0);
	table->insert(t2.s, t2.p, 1);
	table->insert(t3.s, t3.p, 2);
	table->insert(t4.s, t4.p, 3);
	table->insert(t5.s, t5.p, 4);
	table->insert(t6.s, t6.p, 5);
	table->insert(t6.s, t6.p, 6);

	table->print_I();

	return 0;
}*/