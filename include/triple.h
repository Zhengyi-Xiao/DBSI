#ifndef TRIPLE_H
#define TRIPLE_H
#include <vector>
#include <unordered_map>

struct root{
    std::unordered_map<int, struct l0*> s_root;
    std::unordered_map<int, struct l1*> p_root;
    std::unordered_map<int, struct l2*> o_root;
};

struct l0{
    std::unordered_map<int, struct l1*> s_children;
};

struct l1{
    std::unordered_map<int, struct l2*> s_children;
    std::unordered_map<int, struct l2*> p_children;
};

struct l2{
    std::unordered_map<int, struct l3*> p_children;
    std::unordered_map<int, struct l3*> o_children;
};

struct l3{
    std::vector<int> o_children;
};

struct Triple {
    int spo[3];
} Triple;

// trie node
struct TrieNode{
    int id;
    std::vector<TrieNode*> s_children, p_children, o_children;
};

struct Trie{
    struct TrieNode *s_root, *p_root, *o_root;
};


#endif
    