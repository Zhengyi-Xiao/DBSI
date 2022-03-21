#ifndef RDFINDEX_H
#define RDFINDEX_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "xxh3.h"
#include "HashTable.h"

#define loading_factor 0.6
#define COPY(result, index, tmp, i) {result.s = tmp[0]; result.p = tmp[1]; result.o = tmp[2];index = tmp[i];}
#define COPY_TRIPLE(result, t) {result.s = t.s; result.p = t.p; result.o = t.o;}
#define COPY_TABLE(result, tmp) {result.s = tmp[0]; result.p = tmp[1]; result.o = tmp[2];}

#define Nsp         3
#define Np          4
#define Nop         5
#define EndOfNode   -1
#define Negect      -1
#define FirstSearch -2
#define EndSearch   -3

#define X           -2
#define Y           -3
#define Z           -4

struct Triple {
    int s, p, o;

    inline bool operator==(Triple t) {
        if (t.s == s && t.p == p && t.o == 0)
           return true;
        else
           return false;
     }
};


class RDF_index{

private:

public:

    int size;
    int num_element;
    int size_Is, size_Io;
    std::vector<int*>* table;
    std::vector<int> Is, Ip, Io;
    HashTable* Isp,* Iop, *Ispo;

    RDF_index();

    int size_of_table();

    void add(struct Triple t);
    void resize();
    void update_Isp(struct Triple t);
    void update_Iop(struct Triple t);
    void update_Ip(struct Triple t);

    void evaluate(struct Triple t, struct Triple& result, int& index, int& sub_index);

    void evaluate_SYZ(struct Triple t, struct Triple& result, int& index);
    void evaluate_SPZ(struct Triple t, struct Triple& result, int& index);
    void evaluate_SPO(struct Triple t, struct Triple& result, int& index);
    void evaluate_XPO(struct Triple t, struct Triple& result, int& index);
    void evaluate_XYO(struct Triple t, struct Triple& result, int& index);
    void evaluate_SYO(struct Triple t, struct Triple& result, int& index);
    void evaluate_XPZ(struct Triple t, struct Triple& result, int& index);

    void evaluate_XYZ(struct Triple t, struct Triple& result, int& index, int& sub_index);
    void evaluate_XXO(struct Triple t, struct Triple& result, int& index, int& sub_index);
    void evaluate_SXX(struct Triple t, struct Triple& result, int& index, int& sub_index);
    void evaluate_XPX(struct Triple t, struct Triple& result, int& index, int& sub_index);
    void evaluate_XXX(struct Triple t, struct Triple& result, int& index, int& sub_index);

    void print_table();
    void print_I(std::vector<int>& vec);
};

#endif