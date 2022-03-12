#ifndef TRIPLE_H
#define TRIPLE_H
#include <vector>


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
    