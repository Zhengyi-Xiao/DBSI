#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stack>
#include <queue>

#include "include/triple.h"

namespace mapExt{
    template<typename myMap>
    std::vector<typename myMap::key_type> Keys(const myMap& m){
        std::vector<typename myMap::key_type> r;
        r.reserve(m.size());
        for (const auto&kvp : m){
            r.push_back(kvp.first);
        }
        return r;
    }

    template<typename myMap>
    bool check_key(const myMap map, int key){
        return (map.find(key) == map.end()) ? false : true;
    }
}

void insert(struct Trie* root, struct Triple triple){
    int s = triple.s; int p = triple.p; int o = triple.o;
    bool l3Flag = false; bool l4Flag = false;

    // create root for s
    if(!mapExt::check_key(root->s_root->children, s))
        root->s_root->children[s] = new struct TrieNode();
    struct TrieNode* l0 = root->s_root->children[s];

    // create root for p
    if(!mapExt::check_key(l0->children, p))
        l0->children[p] = new struct TrieNode();
    struct TrieNode* l1 = l0->children[p];
    
    if(!mapExt::check_key(root->p_root->children, p)){
        root->p_root->children[p] = l1;
        l1->children[o] = new struct TrieNode();
    }
    else{
        l1->children[o] = new struct TrieNode();
        if(!mapExt::check_key(root->p_root->children, o))
            root->p_root->children[p]->children[o] = l1->children[o];
        else
            l3Flag = true;
    }
    struct TrieNode* l2 = l1->children[o];

    if(!mapExt::check_key(root->o_root->children, o)){
        root->o_root->children[o] = l2;
        l2->children[s] = new struct TrieNode();
    }
    else{
        l2->children[s] = new struct TrieNode();
        if(!mapExt::check_key(root->o_root->children, s))
            root->o_root->children[o]->children[s] = l2->children[s];
        else
            l4Flag = true;
    }
    struct TrieNode* l3 = l2->children[s];
    
    l3->children[p] = new struct TrieNode();
    struct TrieNode* l4 = l3->children[p];
    
    if(l3Flag){
        root->p_root->children[p]->children[o]->children[s] = l3;
    }
    if(l4Flag){
        root->o_root->children[o]->children[s]->children[p] = l4;
    }
}

void get_all_keys(std::unordered_map<int, struct TrieNode*> map){
    std::vector<int> keys;
    keys.reserve(map.size());
    for(auto kv : map) {
        keys.push_back(kv.first);
        std::cout << kv.first << "  ";
    } 
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

    insert(root, t1);
    insert(root, t2);
    insert(root, t3);
    //insert(root, t4);
    //insert(root, t5);
    //insert(root, t6);

    print(root->s_root);
    //get_all_keys(root->p_root->children);

    /*
    get_all_keys(root->s_root->children);
    get_all_keys(root->s_root->children[2]->children);
    get_all_keys(root->s_root->children[2]->children[3]->children);
    get_all_keys(root->s_root->children[2]->children[3]->children[2]->children);
    get_all_keys(root->s_root->children[2]->children[3]->children[2]->children[2]->children);
   */
    //std::cout << mapExt::check_key(l1, 3) << std::endl;    
    //std::cout << mapExt::check_key(l2, 2) << std::endl;    
    //std::cout << mapExt::check_key(l3, 2) << std::endl;   
    /*
    struct Triple t1 = {2, 3, 2};
    struct Triple t2 = {2, 3, 4};
    struct Triple t3 = {4, 3, 2};
    struct Triple t4 = {2, 3, 5};
    struct Triple t5 = {4, 3, 5};

    TrieNode* s_root = new TrieNode();
    TrieNode* p_root = new TrieNode();
    TrieNode* o_root = new TrieNode();

    Trie* root = new Trie();
    root->s_root = s_root;
    root->p_root = p_root;
    root->o_root = o_root;


    insert_s(root, t1);
    //insert_s(root, t2);
    //insert_s(root, t3);
    //insert_s(root, t4);
    //insert_s(root, t5);

    std::cout << "===============" << std::endl;

    print_s(s_root);

    std::cout << "================print p root===============" << std::endl;
    //insert_p(root, t1);
    print_p(p_root);    
    */
    return 0;
}
