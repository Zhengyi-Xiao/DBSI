#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <queue>
#include "include/triple.h"



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

int main(){
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

    return 0;
}
