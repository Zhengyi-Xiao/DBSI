#include "include/HashTable.h"
#include "include/xxh3.h"
#include <vector>

HashTable::HashTable(int tuple_type){
	this->capacity = 1024;
    this->num_element = 0;
	this->table = new std::vector<std::vector<int>>;
	this->table->resize(capacity);
    this->tuple_type = tuple_type;
}

void HashTable::insert(int s, int p, int o, int value){
    int i;
    if(tuple_type == 3){
        char concatenated[12];
        std::memcpy(concatenated, (char*)&s, sizeof(int));
        std::memcpy(concatenated+4, (char*)&p, sizeof(int));
        std::memcpy(concatenated+8, (char*)&o, sizeof(int));
        i = XXH32(concatenated, 12, 0) % this->capacity;

        if((float)this->num_element/this->capacity > loading_factor)
            resize();

        std::vector<int> T = this->table->at(i);
        while(!T.empty()){
            i = (i + 1) % this->capacity;
            T = this->table->at(i);
        }
        std::vector<int> T_new = {s, p, o, value};
        this->table->at(i) = T_new;
        ++this->num_element;
    }
    else{
        char concatenated[8];
        std::memcpy(concatenated, (char*)&s, sizeof(int));
        std::memcpy(concatenated+4, (char*)&p, sizeof(int));
        i = XXH32(concatenated, 8, 0) % this->capacity;   
        if((float)this->num_element/this->capacity > loading_factor)
            resize();

        std::vector<int> T = this->table->at(i);
        while(!T.empty()){
            i = (i + 1) % this->capacity;
            T = this->table->at(i);
        }
        std::vector<int> T_new = {s, p, value};
        this->table->at(i) = T_new;
        ++this->num_element;     
    }

}

int HashTable::search(int s, int p, int o){
    int i;
    if(tuple_type == 3){
        char concatenated[12];
        std::memcpy(concatenated, (char*)&s, sizeof(int));
        std::memcpy(concatenated+4, (char*)&p, sizeof(int));
        std::memcpy(concatenated+8, (char*)&o, sizeof(int));
        i = XXH32(concatenated, 12, 0) % this->capacity;
        
        if (this->table->at(i).empty()){
            return -1;
        }
        else{
            std::vector<int> T = this->table->at(i);
            while(!T.empty()){
                if((T.at(0) == s) && (T.at(1) == p) && (T.at(2) == o)){
                    return T.at(3);
                }
                i = (i + 1) % this->capacity;
                T = this->table->at(i);
            }
        }
    }
    else{
        char concatenated[8];
        std::memcpy(concatenated, (char*)&s, sizeof(int));
        std::memcpy(concatenated+4, (char*)&p, sizeof(int));
        i = XXH32(concatenated, 8, 0) % this->capacity;     
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
    }

	return -1;
}

void HashTable::resize(){
    if(this->tuple_type == 2){
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
    else{
        std::vector<std::vector<int>>* copy = new std::vector<std::vector<int>>;
        int new_size = this->capacity << 1;
        copy->resize(new_size);
        char concatenated[12];

        for(int j = 0; j < this->capacity; j++){
            if(!this->table->at(j).empty()){
                std::memcpy(concatenated, (char*)&this->table->at(j)[0], sizeof(int));
                std::memcpy(concatenated+4, (char*)&this->table->at(j)[1], sizeof(int));
                std::memcpy(concatenated+8, (char*)&this->table->at(j)[2], sizeof(int));

                int i = XXH32(concatenated, 12, 0) % new_size;
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
 
}

void HashTable::print_I(){
    if(this->tuple_type == 2){
        for(int i = 0; i < this->table->size(); i++){
            if(!this->table->at(i).empty()){
                std::cout << i << ": " << this->table->at(i)[0] << "-" << this->table->at(i)[1] << "-" << this->table->at(i)[2] << " ";
            }
            else
                std::cout << " " << " ";
        }
        std::cout << std::endl;
    }
    else{
        for(int i = 0; i < this->table->size(); i++){
            if(!this->table->at(i).empty()){
                std::cout << i << ": " << this->table->at(i)[0] << "-" << this->table->at(i)[1] << "-" << this->table->at(i)[2] << "-" << this->table->at(i)[3] << " ";
            }
            else
                std::cout << " " << " ";
        }
        std::cout << std::endl; 
    }

}

HashTable::~HashTable(){
	delete[] table;
	table = NULL;
}
