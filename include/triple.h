#ifndef TRIPLE_H
#define TRIPLE_H
#include <vector>
#include <unordered_map>

#define X -1
#define Y -2
#define Z -3

struct Query{
    int s, p, o;
};

struct Triple {
    int s, p, o;
};

struct TrieNode{
    std::unordered_map<int, struct TrieNode*> children;
};

struct Trie{
    struct TrieNode *s_root, *p_root, *o_root;
};

bool check_key(const std::unordered_map<int, struct TrieNode*> map, int key);
void add(struct Trie* root, struct Triple triple);

#endif
    