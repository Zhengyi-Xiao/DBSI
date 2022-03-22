#ifndef RDFINDEX_H
#define RDFINDEX_H

#include <iostream>
#include <unordered_map>
#include <vector>

#include "xxh3.h"

#define loading_factor 0.6
#define COPY(result, index, tmp, i) {result.s = tmp[0]; result.p = tmp[1]; result.o = tmp[2];index = tmp[i];}
#define COPY_TRIPLE(result, t) {result.s = t.s; result.p = t.p; result.o = t.o;}
#define COPY_TABLE(result, tmp) {result.s = tmp[0]; result.p = tmp[1]; result.o = tmp[2];}

#define Nsp         3
#define Np          4
#define Nop         5
#define EndOfNode   -1
#define FirstSearch -2
#define EndSearch   -3

#define INITIAL_CAP 16384

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

struct key_hash : public std::unary_function<Triple, std::size_t>{
    std::size_t operator()(const Triple& t) const{
        char concatenated[12];
        std::memcpy(concatenated, (char*)&t.s, sizeof(int));
        std::memcpy(concatenated+4, (char*)&t.p, sizeof(int));
        std::memcpy(concatenated+8, (char*)&t.o, sizeof(int));
        return XXH32(concatenated, 12, 0); // XXHASH is used
    }
};
 
struct key_equal : public std::binary_function<Triple, Triple, bool>{
    bool operator()(const Triple& t1, const Triple& t2) const{
        return (t1.s == t2.s && t1.p == t2.p && t1.o == t2.o);
    }
};

typedef std::unordered_map<const Triple, int, key_hash, key_equal> map_t;

class RDF_index{

private:
    int num_element = 0;
    int size_Is = 0; 
    int size_Io = 0;
    std::vector<int*>* table;
    std::vector<int> *Is, *Ip, *Io;
    map_t *Isp, *Iop, *Ispo;

public:
    RDF_index();

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

    int size_of_table();

};

#endif