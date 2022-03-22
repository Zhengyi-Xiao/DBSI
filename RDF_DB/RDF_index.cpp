#include "../include/RDF_index.h"

RDF_index::RDF_index(){
    this->table = new std::vector<int*>;
    this->table->resize(INITIAL_CAP);
    this->Is = new std::vector<int>;
    this->Is->resize(INITIAL_CAP);
    this->Ip = new std::vector<int>;
    this->Ip->resize(INITIAL_CAP);
    this->Io = new std::vector<int>;
    this->Io->resize(INITIAL_CAP);            

    this->Isp  = new map_t();
    this->Iop  = new map_t();
    this->Ispo = new map_t();
}

// Return the size of the RDF index table
int RDF_index::size_of_table(){
    return this->num_element;
}

// Resize the RDF index table and three index lists
void RDF_index::resize(){
    int current_size = this->table->size();
    this->table->resize(current_size << 1);
    this->Is->resize(current_size << 1);    
    this->Ip->resize(current_size << 1);
    this->Io->resize(current_size << 1);
}

// Update Isp index
inline void RDF_index::update_Isp(struct Triple t){
    t.o = 0;
    if((*this->Is)[t.s] == 0){ // if it is the first occurrence， create a new index for Is also
        (*this->Is)[t.s] = this->num_element;
        (*this->Isp)[t] = this->num_element;
        ++this->size_Is;
    }
    else if(this->Isp->find(t) != this->Isp->end()){ // Algorithm 4 in the paper
        if((*this->table)[(*this->Isp)[t] - 1][Nsp] != EndOfNode)
            (*this->table)[this->num_element - 1][Nsp] = (*this->table)[(*this->Isp)[t] - 1][Nsp];
        (*this->table)[(*this->Isp)[t] - 1][Nsp] = this->num_element - 1;
    }
    else{ // special case when only the head of the linked list exists, in such case, makes the new node the next
        (*this->Isp)[t] = this->num_element;
        (*this->table)[this->num_element - 1][Nsp] = (*this->Is)[t.s] - 1;
        (*this->Is)[t.s] = this->num_element;
    }
}

// Update Iop index
inline void RDF_index::update_Iop(struct Triple t){
    t.s = 0;
    if((*this->Io)[t.o] == 0){  // if it is the first occurrence， create a new index for Io also
        (*this->Io)[t.o] = this->num_element;
        (*this->Iop)[t] = this->num_element;
        ++this->size_Io;
    }
    else if(this->Iop->find(t) != this->Iop->end()){ // Algorithm 4 in the paper
        if((*this->table)[(*this->Iop)[t] - 1][Nop] != EndOfNode)
            (*this->table)[this->num_element - 1][Nop] = (*this->table)[(*this->Iop)[t] - 1][Nop];
        (*this->table)[(*this->Iop)[t] - 1][Nop] = this->num_element - 1;
    }
    else{ // special case when only the head of the linked list exists, in such case, makes the new node the next
        (*this->Iop)[t] = this->num_element;
        (*this->table)[this->num_element - 1][Nop] = (*this->Io)[t.o] - 1;
        (*this->Io)[t.o] = this->num_element;
    }
}

// Update the Ip index list
inline void RDF_index::update_Ip(struct Triple t){
    if((*this->Ip)[t.p] == 0){ // speical case when there is no element in the list
        (*this->Ip)[t.p] = this->num_element;
    }
    else{ // Algorithm 4 in the paper
        if((*this->table)[(*this->Ip)[t.p] - 1][Np] != EndOfNode)
            (*this->table)[this->num_element - 1][Np] = (*this->table)[(*this->Ip)[t.p] - 1][Np];
        (*this->table)[(*this->Ip)[t.p] - 1][Np] = this->num_element - 1;
    }
}

// Add(s, p, o) that allows inserting triples into the database
void RDF_index::add(struct Triple t){
    int* T_new = new int[6];
    T_new[0] = t.s;        T_new[1] = t.p;        T_new[2] = t.o;
    T_new[3] = EndOfNode;  T_new[4] = EndOfNode;  T_new[5] = EndOfNode;
    
    // Check if (s, p, o) is in the DB already by checking Ispo index
    // If the triple is already in the DB, skip it. (Duplicate Removal)
    if(this->Ispo->find(t) != this->Ispo->end())
        return;

    // Resize the table when the table is not large enough
    if(this->table->size() < (this->num_element + 2))
        resize();

    (*this->table)[this->num_element] = T_new;

    ++this->num_element;

    // update all six index lists accordingly
    update_Isp(t);
    update_Iop(t);
    update_Ip(t);
    this->Ispo->emplace(t, this->num_element);
}

// The following evaluate functions are implemented strictly based on what paper described.
// Therefore, only the first one is commented.
// Input:  Triple Pattern: (s, p, o)
// Output: Result: (s', p', o')
//         Index: pointer to the next
inline void RDF_index::evaluate_SYZ(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            if((*this->Is)[t.s] == 0){ // if s is not in the Is list, return
                index = EndSearch;
                return;
            } // otherwise, find the head of the Is list in the table, copy the value to the output
            COPY(result, index, (*this->table)[(*this->Is)[t.s] - 1], Nsp);
        }
        else{ // find the next pointer by reading Nsp column in the table
            COPY(result, index, (*this->table)[index], Nsp);
        }
    } while (t.p == t.o && result.p != result.o && index > EndOfNode);
    // if the result does not match the pattern, return EndSearch
    if(t.p == t.o && result.p != result.o)
        index = EndSearch;
}

inline void RDF_index::evaluate_SPZ(struct Triple t, struct Triple& result, int& index){
    t.o = 0;
    if((*this->Isp)[t] == 0){
        index = EndSearch;
        return;
    }
    if(index == FirstSearch){
        COPY(result, index, (*this->table)[(*this->Isp)[t] - 1], Nsp);
    }
    else{
        COPY(result, index, (*this->table)[index], Nsp);
    }
    if(result.s != t.s || result.p != t.p)
        index = EndSearch;
}

inline void RDF_index::evaluate_XPO(struct Triple t, struct Triple& result, int& index){
    t.s = 0;
    if((*this->Iop)[t] == 0){
        index = EndSearch;
        return;
    }
    if(index == FirstSearch){
        COPY(result, index, (*this->table)[(*this->Iop)[t] - 1], Nop);
    }
    else{
        COPY(result, index, (*this->table)[index], Nop);
    }
    if(result.o != t.o || result.p != t.p)
        index = EndSearch;
}

inline void RDF_index::evaluate_SPO(struct Triple t, struct Triple& result, int& index){
    if((*this->Ispo)[t] != 0){
        COPY_TRIPLE(result, t);
        index = EndOfNode;
    }
    else{
        index = EndSearch;
    }
}

inline void RDF_index::evaluate_XYO(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            if((*this->Io)[t.o] == 0){
                index = EndSearch;
                return;
            }
            COPY(result, index, (*this->table)[(*this->Io)[t.o] - 1], Nop);
        }
        else{
            COPY(result, index, (*this->table)[index], Nop);
        }
    } while (t.p == t.s && result.p != result.s && index > EndOfNode);

    if(t.p == t.s && result.p != result.s)
        index = EndSearch;
}

inline void RDF_index::evaluate_SYO(struct Triple t, struct Triple& result, int& index){
   if(this->size_Is < this->size_Io){
        do{
            if(index == FirstSearch){
                if((*this->Is)[t.s] == 0){
                    index = EndSearch;
                    return;
                }
                COPY(result, index, (*this->table)[(*this->Is)[t.s] - 1], Nsp);
            }
            else{
                COPY(result, index, (*this->table)[index], Nsp);       
            }
        } while(t.o != result.o && index > EndOfNode);
        if(t.o != result.o)
            index = EndSearch;
    }
    else{
        do{
            if(index == FirstSearch){
                if((*this->Io)[t.o] == 0){
                    index = EndSearch;
                    return;
                }
                COPY(result, index, (*this->table)[(*this->Io)[t.o] - 1], Nop);
            }
            else{
                COPY(result, index, (*this->table)[index], Nop);
            }
        } while(t.s != result.s && index > EndOfNode);
        if(t.s != result.s)
            index = EndSearch;
    }
}

inline void RDF_index::evaluate_XPZ(struct Triple t, struct Triple& result, int& index){
    do{
        if(index == FirstSearch){
            if((*this->Ip)[t.p] == 0){
                index = EndSearch;
                return;
            }
            COPY(result, index, (*this->table)[(*this->Ip)[t.p] - 1], Np);
        }
        else{
            COPY(result, index, (*this->table)[index], Np);
        }
    } while (t.s == t.o && result.s != result.o && index > EndOfNode);
    if(t.s == t.o && result.s != result.o)
        index = EndSearch;       
}

// Instead of matching the pattern <x, y, z> by iterating over the triple table,
// I reused EVALUTE functions in the previous part. For example, to math the 
// pattern <s, x, x>, we fisrt iterate Is list, and for each index in the Is list
// we evaluat(s, x, x). The rest follows.
inline void RDF_index::evaluate_SXX(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)    // this is weird but is necessary somehow
        index = 1;
    if(sub_index == EndOfNode){
        ++index;
        sub_index = FirstSearch;
    }
    // iterate Is list
    for(; index < this->Is->size(); index++){ 
        if((*this->Is)[index] > 0){
            // for each s in Is list, search parttern (s, x, x)
            evaluate_SYZ({index, X, X}, result, sub_index);
            if(sub_index != EndSearch)
                return;
        }
        sub_index = FirstSearch;
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XXO(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)
        index = 1;
    if(sub_index == EndOfNode){
        ++index;
        sub_index = FirstSearch;
    }
    for(; index < this->Io->size(); index++){
        if((*this->Io)[index] > 0){
            evaluate_XYO({X, X, index}, result, sub_index);
            if(sub_index != EndSearch)
                return;
        }
        sub_index = FirstSearch;
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XPX(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)
        index = 1;
    if(sub_index == EndOfNode){
        ++index;
        sub_index = FirstSearch;
    }

    for(; index < this->Ip->size(); index++){
        if((*this->Ip)[index] > 0){
            evaluate_XPZ({X, index, X}, result, sub_index);
            if(sub_index != EndSearch)
                return;
        }
        sub_index = FirstSearch;
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XXX(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch)
        index = 1;

    for(; index < this->Is->size(); index++){
        if((*this->Is)[index] > 0){
            evaluate_SPO({index, index, index}, result, sub_index);
            if(sub_index == EndOfNode){
                ++index;
                return;
            }
        }
    }
    index = EndSearch;
}

inline void RDF_index::evaluate_XYZ(struct Triple t, struct Triple& result, int& index, int& sub_index){
    if(index == FirstSearch){
        COPY_TABLE(result, (*this->table)[0]);
        index = 1;
    }
    else{
        COPY_TABLE(result, (*this->table)[index]);
        ++index;
    }   
    if(index >= this->num_element)
        index = EndOfNode;     
}

// The start of the EVALUATE fucntion
void RDF_index::evaluate(struct Triple t, struct Triple& result, int& index, int& sub_index){

    // To match different patterns and send to different evolution functions
    // P.S. I used to have a better-readable-form like the lines after XYZ, but I found this is faster...
    // And that's true, in XYZ, I need exhaustedly match all patterns, but changing to a tree like 
    // structure, it is reduced to 3 comparison as in the first part.
    if(t.s >= 0){
        t.p >= 0 ? (t.o >= 0 ? evaluate_SPO(t, result, index) : evaluate_SPZ(t, result, index)) : (t.o >= 0 ? evaluate_SYO(t, result, index) : evaluate_SYZ(t, result, index));
        return;
    }
    else{
        if(t.p >= 0){
            t.o >= 0 ?  evaluate_XPO(t, result, index) : evaluate_XPZ(t, result, index);
            return;
        }
        else{
            if(t.o >= 0){
                evaluate_XYO(t, result, index); // ?X?YO
                return;                
            }
            else{ // XYZ
                if(t.s != t.p && t.s != t.o && t.p != t.o){
                    evaluate_XYZ(t, result, index, sub_index);    
                }
                else if(t.s == t.p && t.s != t.o && t.p != t.o){
                    evaluate_XXO(t, result, index, sub_index);
                }
                else if(t.p == t.o && t.s != t.p && t.s != t.o){
                    evaluate_SXX(t, result, index, sub_index);
                }
                else if(t.s == t.o && t.s != t.p && t.o != t.p){
                    evaluate_XPX(t, result, index, sub_index);
                }
                else if(t.s == t.p && t.p == t.o && t.o == t.s){
                    evaluate_XXX(t, result, index, sub_index);
                }
                return;
            }
        }
    }

}