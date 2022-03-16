#ifndef TRIPLE_H
#define TRIPLE_H
#include <vector>
#include <unordered_map>

#define V -1
#define X -2
#define Y -3
#define Z -4

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

struct State {
    int start;
    int result[3]; 
    std::unordered_map<int, struct TrieNode*>::iterator iter[3];
};

State make();

void next(struct Trie* root, State& state);
void next_helper(struct TrieNode* current, int depth, struct State& state);


bool isDone(State const& state);

//void helper(struct TrieNode* current, int depth, int* output);

bool check_key(const std::unordered_map<int, struct TrieNode*> map, int key);
void add(struct Trie* root, struct Triple triple);

#endif
    