#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>

#define loading_factor 0.6

class HashTable{

public:
	HashTable();

	void insert(int s, int p, int value);
	int search(int s, int p);
	void resize();
	void print_I();
	int getCapacity() const;
	~HashTable();

	int hashValue(int key) const;
	
	std::vector<std::vector<int> > *table;				//pointer to the hash table
    int num_element;		//number of items in the hash table
    int capacity;			//maximum length of the hash table	
};

#endif