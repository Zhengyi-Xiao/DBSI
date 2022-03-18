#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>

#define loading_factor 0.6

class HashTable{
private:
	std::vector<std::vector<int> > *table;		// pointer to the hash table
	int num_element;							// number of items in the hash table
	int capacity;								// maximum length of the hash table	
	int tuple_type;								// if tuple_type = 2, then we have a hash map on a two-tuple

public:
	HashTable(int tuple_type);
	void insert(int s, int p, int o, int value);
	int search(int s, int p, int o);
	void resize();
	void print_I();
	~HashTable();
};

#endif