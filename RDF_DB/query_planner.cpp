#include "../include/query_planner.h"

struct Triple NULL_TRIPLE =  {-1, -1, -1};

Query_planner::Query_planner(class Turtle_handler* Turtle_handler){
    this->Turtle_handler = Turtle_handler;
}

/*
The greedy plan is partially implemented based on the model answer. 
To see what I changed, please see the report. It is still O(N)
*/
void Query_planner::plan_query(std::vector<struct Triple>& U, std::vector<struct Triple>& P){
    std::vector<int> B; std::vector<int> variables; std::vector<int> v_intersection;
    while(!U.empty()){
        struct Triple t_best = NULL_TRIPLE; int score_best = 100; bool intersected = false;
        for(auto t : U){
            int score = this->get_score(t, B);
            this->var(t, variables); this->intersection(variables, B, v_intersection);
            /* Model Answer
            if((t_best.s == -1 && t_best.p == -1 && t_best.o == -1) || 
                (score < score_best && (variables.empty() || !v_intersection.empty()))){
                    t_best = t; score_best = score;
            }       
            */
            if(t_best.s == -1 && t_best.p == -1 && t_best.o == -1){ // we take the first triple pattern regardless of what
                intersected = !v_intersection.empty();
                t_best = t; score_best = score;
            }
            if(score == score_best){
                if(t.s < 0){ // if they are in pattern <X, p, o> or <s, p, X>, it is a special case we need to handle as I stated in the report
                    if((t_best.s < 0 && (*this->Turtle_handler->table->Iop)[{0, t.p, t.o}].size < (*this->Turtle_handler->table->Iop)[{0, t_best.p, t_best.o}].size) ||
                       (t_best.o < 0 && (*this->Turtle_handler->table->Iop)[{0, t.p, t.o}].size < (*this->Turtle_handler->table->Isp)[{t_best.s, t_best.p, 0}].size)){
                        t_best = t; score_best = score;
                    }
                }
                if(t.o < 0){
                    if((t_best.o < 0 && (*this->Turtle_handler->table->Isp)[{t.s, t.p, 0}].size < (*this->Turtle_handler->table->Isp)[{t_best.s, t_best.p, 0}].size) || 
                       (t_best.s < 0 && (*this->Turtle_handler->table->Isp)[{t.s, t.p, 0}].size < (*this->Turtle_handler->table->Iop)[{0, t_best.p, t_best.o}].size)){
                        t_best = t; score_best = score;
                    }
                }
            }            
            if(intersected){
                if(score < score_best && (variables.empty() || !v_intersection.empty())){
                    t_best = t; score_best = score;
                }
            }
            else{ // we always want an intersection
                if((variables.empty() || !v_intersection.empty())){
                    t_best = t; score_best = score; intersected = true;
                }                
            }
        }
        P.push_back(t_best);
        this->var(t_best, variables);
        this->union_sets(B, variables, B);
        this->difference(U, t_best);
    }
    
}

// (s, p, o) ≺ (s, ?, o) ≺ (?, p, o) = (s, p, ?) ≺ (?, ?, o) ≺ (s, ?, ?) ≺ (?, p, ?) ≺ (?, ?, ?)
int Query_planner::get_score(struct Triple t, const std::vector<int> B){
    // the variables in B are considered bound for the given triple t.
    if (std::count(B.begin(), B.end(), t.s))
        t.s = 1;
    if (std::count(B.begin(), B.end(), t.p))
        t.p = 1;
    if (std::count(B.begin(), B.end(), t.o))
        t.o = 1;

    // depending on the type, return the score
    if(t.s < 0)
        if(t.p < 0)
            return (t.o < 0 ? XXX : XXO);
        else
            return (t.o < 0 ? XPX : XPO);
    else
        if(t.p < 0)
            return (t.o < 0 ? SXX : SXX);
        else
            return (t.o < 0 ? SPX : SPO);
}

// return the varialbes in the triple t.
void Query_planner::var(const struct Triple t, std::vector<int>& variables){
    variables.clear();
    if(t.s < 0)
        variables.push_back(t.s);
    if(t.p < 0)
        variables.push_back(t.p);
    if(t.o < 0)
        variables.push_back(t.o);
}

// return the intersection of the two vectors, v1 and v2. 
void Query_planner::intersection(std::vector<int> v1, std::vector<int> v2, std::vector<int>& v_intersection){
    v_intersection.clear();
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
 
    std::set_intersection(v1.begin(), v1.end(),
                          v2.begin(), v2.end(),
                          std::back_inserter(v_intersection));
}

// return the union of the two vectors, v1, and v2.
void Query_planner::union_sets(std::vector<int> v1, std::vector<int> v2, std::vector<int>& v_union){
    v_union.resize(v1.size() + v2.size());
    
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    std::vector<int>::iterator it=std::set_union (v1.begin(), v1.end(), 
                                                  v2.begin(), v2.end(), 
                                                  v_union.begin());

    v_union.resize(it-v_union.begin());
}

// return the difference of the vector and a triple
void Query_planner::difference(std::vector<struct Triple>& U, struct Triple t){
    for(auto iter = U.begin(); iter != U.end(); ++iter){
        if(iter->s == t.s && iter->p == t.p && iter->o == t.o){
            iter = U.erase(iter);
            break;
        }
    }
}