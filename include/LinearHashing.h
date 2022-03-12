#ifndef LINEARHASHING_H
#define LINEARHASHING_H

#include <vector>
#include <iostream>
#include <math.h>  
#include "../include/xxh3.h"

class Bucket{
	std::vector <int> values;
	Bucket* overflow;
	int size;

	public:
        Bucket(int size){
            this->size = size;
            overflow = NULL;
        }
        void insert(int key);
        void copy(std::vector <int> &temp);
        int search(int key);
        void display();
        int del(int key);
        bool isEmpty();
        bool isOverflow();
};

class Hashmap{
	int numrecords;
	int next;
	int hash (int n);
	void reinsert(int bucket_num);
	int numbits;
	int bucketsize;
	std::vector<Bucket *>buckets;

public:
	Hashmap(int numbits, int bucketsize){
		this->numbits = numbits;
		this->bucketsize = bucketsize;
		next = 0;
		numrecords = 1 << numbits;
		for(int i = 0; i < numrecords; i++)
			buckets.push_back(new Bucket(bucketsize));
	}
	void insert(int key);
	void reinsert(std::vector <int> &temp);
	int search(int key);
	void display();
};


#endif

