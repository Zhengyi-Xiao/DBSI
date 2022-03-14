#ifndef TRIPLE_H
#define TRIPLE_H
#include <vector>
#include <unordered_map>

struct Triple {
    int s,p,o;
} Triple;

// trie node
struct TrieNode{
    std::unordered_map<int, struct TrieNode*> children;
};

struct Trie{
    struct TrieNode *s_root, *p_root, *o_root;
};


#endif
    