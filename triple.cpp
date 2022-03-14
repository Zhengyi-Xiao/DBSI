#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <queue>
#include "include/triple.h"

/*

struct TrieNode* seek(struct TrieNode *root, struct Triple triple, int offset){
    struct TrieNode *pCrawl = root;
    for(int i = 0; i < offset+1; i++){
        for(const auto& child: pCrawl->s_children){
            if(child->id == triple.spo[i]){
                pCrawl = child;
                break;
            }
        }
    }
    return pCrawl;
}

void updateRoot_p(struct TrieNode *root, struct TrieNode *newNode){
    for(const auto& child: root->p_children){
        if(child->id == newNode->id){
            return;
        }
    }
    bool inserted = false;
    int counter = 0;
    for(const auto& child: root->p_children){ // do insertion on an appropriate postion
        if(child->id > newNode->id){
            inserted = true;
            root->p_children.insert(root->p_children.begin()+counter, newNode);
        }
        counter++;
    }
    if(!inserted){}
        root->p_children.push_back(newNode);
}

void insert_s(struct Trie *root, struct Triple triple){
    struct TrieNode *pCrawl = root->s_root;

    std::vector<TrieNode*> spo;
    for (int i = 0; i < 3; i++){
        bool find = false;
        for(const auto& child: pCrawl->s_children){ // locate the node with spo[i] in s_children
            if(child->id == triple.spo[i]){
                find = true;
                pCrawl = child;
                spo.push_back(child);
                break;
            }
        }
        if(!find){
            struct TrieNode* newChild = new TrieNode();
            newChild->id = triple.spo[i];          
            spo.push_back(newChild);
            bool inserted = false;
            int counter = 0;
            for(const auto& child: pCrawl->s_children){
                if(child->id > triple.spo[i]){
                    inserted = true;
                    pCrawl->s_children.insert(pCrawl->s_children.begin()+counter, newChild);
                    break;
                }
                counter++;
            }
            if(!inserted)
                pCrawl->s_children.push_back(newChild);
            pCrawl = newChild;
        }
    }
    
    updateRoot_p(root->p_root, spo[1]); // update root for index on p

    //root->p_root->p_children.push_back(spo[1]);
    spo[1]->p_children.push_back(spo[2]);
    spo[2]->p_children.push_back(spo[0]);
    //std::cout << spo[0]->id << "  " << spo[1]->id << "  " << spo[2]->id <<std::endl;
}

void insert_p(struct Trie *root, struct Triple triple){
    struct TrieNode *pCrawl = seek(root->s_root, triple, 1); // loacte the start on s
    updateRoot_p(root->p_root, pCrawl); // update root for index on p

    // locate the node in children in s-based index
    for(const auto& child: pCrawl->s_children){
        if(child->id == triple.spo[2]){ // if they are equal
            struct TrieNode* newChild = new TrieNode();
            newChild->id = triple.spo[2];          
            bool inserted = false;
            int counter = 0;
            for(const auto& child: pCrawl->p_children){ // do insertion on an appropriate postion
                if(child->id > triple.spo[2]){
                    inserted = true;
                    pCrawl->p_children.insert(pCrawl->p_children.begin()+counter, newChild);
                }
                counter++;
            }
            if(!inserted){}
                pCrawl->p_children.push_back(newChild);
            pCrawl = newChild;
            break;
        }
    }        

}

void print_s(struct TrieNode* root){
    std::queue<TrieNode*> myqueue;
    myqueue.push(root);

    // Printing content of queue
    while (!myqueue.empty()) {
        TrieNode* top = myqueue.front();
        for(const auto& child: top->s_children){
            myqueue.push(child);
        }
        std::cout << top->id << std::endl;
        myqueue.pop();

    }   
}

void print_p(struct TrieNode* root){
    std::queue<TrieNode*> myqueue;
    myqueue.push(root);

    // Printing content of queue
    while (!myqueue.empty()) {
        TrieNode* top = myqueue.front();
        for(const auto& child: top->p_children)
            myqueue.push(child);
        std::cout << top->id << std::endl;
        myqueue.pop();

    }   
}
*/

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

bool check_elm(std::vector<int> v, int key){
    return (std::find(v.begin(), v.end(), key) == v.end()) ? false : true;
}

void get_all_keys(std::unordered_map<int, struct l2*> map){
    std::vector<int> keys;
    keys.reserve(map.size());

    for(auto kv : map) {
        keys.push_back(kv.first);
        std::cout << kv.first <<std::endl;
    } 
}


void insert(struct root* root, struct Triple triple){
    struct l1* l1;
    struct l2* l2;
    struct l3* l3;

    if(!mapExt::check_key(root->s_root, triple.spo[0]))
        root->s_root[triple.spo[0]] = new struct l0();
    struct l0* l0 = root->s_root[triple.spo[0]];

    if(!mapExt::check_key(l0->s_children, triple.spo[1])){
        l1 = new struct l1();
        l0->s_children[triple.spo[1]] = l1;
        if(!mapExt::check_key(root->p_root, triple.spo[1]))
            root->p_root[triple.spo[1]] = l1;
    }
    else{
        l1 = l0->s_children[triple.spo[1]];
    }
    
    if(!mapExt::check_key(l1->s_children, triple.spo[2])){
        l2 = new struct l2();
        l1->s_children[triple.spo[2]] = l2;
        if(mapExt::check_key(root->p_root, triple.spo[1]))
            root->p_root[triple.spo[1]] = l1;
        else
            l1->p_children[triple.spo[2]] = l2;
        if(!mapExt::check_key(root->o_root, triple.spo[2]))
            root->o_root[triple.spo[2]] = l2;
    }
    else{
        l2 = l1->s_children[triple.spo[2]];
    }

    if(!mapExt::check_key(l2->p_children, triple.spo[0])){
        l3 = new struct l3();
        l2->p_children[triple.spo[0]] = l3;
        if(mapExt::check_key(root->o_root, triple.spo[2]))
            root->o_root[triple.spo[2]] = l2;
        else
            l2->o_children[triple.spo[0]] = l3;
    }
    else{
        l3 = l2->p_children[triple.spo[0]];
    }
    l3->o_children.push_back(triple.spo[0]);
    
}

int main(){
    struct Triple t1 = {2, 3, 2};

    struct root* root = new struct root();
    struct test{
        std::unordered_map<int, std::vector<test*>> list;
    };
    struct test* a = new struct test();
    struct test* b = new struct test();
    a->list[0].push_back(b);

    insert(root, t1);

    struct l0* l0 = root->s_root[2];
    struct l1* l1 = root->s_root[2]->s_children[3];
    struct l2* l2 = root->s_root[2]->s_children[3]->s_children[2];
    struct l3* l3 = root->s_root[2]->s_children[3]->s_children[2]->o_children[2];

    std::cout << mapExt::check_key(root->s_root, 2) << std::endl;
    std::cout << mapExt::check_key(root->s_root[2]->s_children, 3) << std::endl;
    std::cout << mapExt::check_key(root->s_root[2]->s_children[3]->s_children, 2) << std::endl;
    std::cout << mapExt::check_key(root->s_root[2]->s_children[3]->s_children[2]->o_children, 2) << std::endl;

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
