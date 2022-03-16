#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stack>
#include <queue>

#include "include/triple.h"

void get_all_keys(std::unordered_map<int, struct TrieNode*> map){
    std::vector<int> keys;
    keys.reserve(map.size());
    for(auto kv : map) {
        keys.push_back(kv.first);
        std::cout << kv.first << "  ";
    } 
}

bool check_key(const std::unordered_map<int, struct TrieNode*> map, int key){
    return (map.find(key) == map.end()) ? false : true;
}

// add a new triple into the trie
void add(struct Trie* root, struct Triple triple){
    int s = triple.s; int p = triple.p; int o = triple.o;
    bool l3Flag = false; bool l4Flag = false;

    // create root for s
    if(!check_key(root->s_root->children, s))
        root->s_root->children[s] = new struct TrieNode();
    struct TrieNode* l0 = root->s_root->children[s];

    // create root for p
    if(!check_key(l0->children, p))
        l0->children[p] = new struct TrieNode();
    struct TrieNode* l1 = l0->children[p];
    
    //  if p_root does not contain p, we want create a new node for p 
    if(!check_key(root->p_root->children, p)){
        root->p_root->children[p] = l1;
        l1->children[o] = new struct TrieNode();
    } // otherwise, it is already there, and there are two cases we need to consider
    else{
        l1->children[o] = new struct TrieNode();
        // when o is not in the p_root, we want set a pointer from p_root from o to l2
        if(!check_key(root->p_root->children[p]->children, o))
            root->p_root->children[p]->children[o] = l1->children[o];
        else // otherwise, it is a special case we need to take care later on
            l3Flag = true;
    }
    struct TrieNode* l2 = l1->children[o];

    // next layer follows above
    if(!check_key(root->o_root->children, o)){
        root->o_root->children[o] = l2;
        l2->children[s] = new struct TrieNode();
    }
    else{
        l2->children[s] = new struct TrieNode();
        if(!check_key(root->o_root->children[o]->children, s))
            root->o_root->children[o]->children[s] = l2->children[s];
        else
            l4Flag = true;
    }
    struct TrieNode* l3 = l2->children[s];
    // last layer included
    l3->children[p] = new struct TrieNode();
    
    if(l3Flag) // special case to handle (s,p,o) where p and o appear before
        root->p_root->children[p]->children[o]->children[s] = l3;

    if(l4Flag)
        root->o_root->children[o]->children[s]->children[p] = l3->children[p];

}


State make(struct Trie* root, struct Query query){
    struct State state;
    state.start = 1;
    state.result[0] = query.s;
    state.result[1] = -1;
    state.result[2] = -1;
    state.iter[0] = root->s_root->children.begin();

    return state;
}

void helper(struct TrieNode* current, int depth, struct State& state){
    if(depth >= 3){
        std::cout << state.result[0] << state.result[1] << state.result[2] << std::endl;
        return;
    }
    for(;state.iter[depth-1]!=current->children.end(); state.iter[depth-1]++){
        state.result[depth] = state.iter[depth-1]->first;
        state.iter[depth] = state.iter[depth-1]->second->children.begin();
        std::cout << state.result[0] << state.result[1] << std::endl;
        //helper(state.iter[depth]->second, depth + 1, state);
        return;
    }
}

void next(struct Trie* root, State& state){
    int output[3];
    helper(root->s_root->children[state.result[0]], state.start, state);
}



void evaluate(struct Trie* root, struct Query query){
    int output[3];
    std::unordered_map<int, struct TrieNode*>::iterator kv = root->s_root->children.begin();

    //helper(root->s_root, 0, output, kv);
}

void print(struct TrieNode* root){
    std::queue<TrieNode*> myqueue;
    myqueue.push(root);
    // Printing content of queue
    while (!myqueue.empty()) {
        TrieNode* top = myqueue.front();

        for(auto kv : top->children) {
            myqueue.push(kv.second);
            std::cout << kv.first << "  ";
        }   
        std::cout << "\n-------" << std::endl;
        myqueue.pop();

    }   
}


int main(){
    struct Triple t1 = {2, 3, 2};
    struct Triple t2 = {2, 3, 4};
    struct Triple t3 = {4, 5, 2};
    struct Triple t4 = {5, 3, 2};
    struct Triple t5 = {2, 5, 2};
    struct Triple t6 = {2, 2, 2};


    struct Trie* root = new struct Trie();
    root->s_root = new struct TrieNode();
    root->p_root = new struct TrieNode();
    root->o_root = new struct TrieNode();

    add(root, t1);
    add(root, t2);
    add(root, t3);
    add(root, t4);
    add(root, t5);
    add(root, t6);

    struct Query q1 = {2, X, Y};
    struct State state = make(root, q1);
    
    next(root, state);
    next(root, state);
    
    //print(root->s_root);
    //get_all_keys(root->o_root->children[4]->children[2]->children);

    return 0;
}
