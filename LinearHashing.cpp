#include "include/LinearHashing.h"
#include <unordered_map>

bool Bucket::isEmpty(){
    return values.size() == 0;
}

bool Bucket::isOverflow(){
   return overflow != NULL;
}

int Bucket::del(int key){
	for(int i = 0; i < values.size(); i++){
		if(values[i] == key){
			values.erase(values.begin() + i);
			if(overflow){
				values.push_back(overflow->values[0]);
				int status = overflow->del(overflow->values[0]);
				if (overflow->isEmpty()){
					delete(overflow);
					overflow = NULL;
				}
			}
			return 1;
		}
	}
	if(overflow){
		int status = overflow->del(key);
		if (overflow->isEmpty()){
			delete(overflow);
			overflow = NULL;
		}
		return status;
	}
	else
		return 0;
}

void Bucket::display(){
	for(int i = 0; i < values.size(); i++)
		std::cout << values[i] << " ";
	if (overflow)
		overflow->display();
	else
		std::cout << std::endl;
}

void Bucket::insert(int key){
	if (values.size() < size)
		values.push_back(key);
	else{
		if (overflow == NULL)
			overflow = new Bucket(size);
		overflow->insert(key);
	}

}

void Bucket::copy(std::vector <int> &temp){
	for(int i = 0; i < values.size(); i++)
		temp.push_back(values[i]);
	values.clear();
	if (overflow){
		overflow->copy(temp);
		delete(overflow);
		overflow = NULL;
	}
}

int Bucket::search(int key){
	for(int i = 0; i < values.size(); i++)
		if (values[i] == key)
			return 1;
	else if (overflow)
		return overflow->search(key);
    return 0;
}

int Hashmap::hash(int n){
	return n&( (1 << numbits) - 1);
}

void Hashmap::reinsert(std::vector <int> &temp){
	for(int i = 0; i < temp.size(); i++){
        int key = temp[i];
		int num = hash(key);
		buckets[num]->insert(key);
	}
}

void Hashmap::insert(int key){
	int num = hash(key);
	if (num >= buckets.size()){
		num = num - (1 << (numbits - 1));
	}
	buckets[num]->insert(key);
	if (!buckets[num]->isOverflow())
		return;
    buckets.push_back(new Bucket(bucketsize));
	numbits = ceil(log2((double)buckets.size()));
	std::vector <int> temp;
	buckets[next]->copy(temp);
	reinsert(temp);
	next++;
	if (next == 1 << (numbits-1)){
		next = 0;
	}
}

void Hashmap::display(){
	std::cout << "INDEX OF NEXT BUCKET TO BE SPLIT " << next << std::endl;
	for(int i = 0; i < buckets.size(); i++){
		std::cout << i << " ";
		buckets[i]->display();
	}
}

int Hashmap::search(int key){
	int num = hash(key);

	if (num >= buckets.size())
		num -= 1 << (numbits - 1);

    int status = buckets[num]->search(key);

    return status ? num : -1;
}


int main(){
    /*
	Hashmap h(0, 5);
    h.insert(3);
    h.insert(5);
    h.insert(8);
    h.insert(4);
    h.insert(6);
    h.insert(9);
    h.insert(7);
    h.insert(15);
    //h.display();

    printf("%u\n", XXH32("It is a test.", 14, 0));
    printf("%u\n", XXH32("It is", 5, 0));
    */
    std::unordered_map<int, std::string> idx2IRI;
    std::unordered_map<std::string, int> IRI2idx;

    std::string token = "<http://swat.cse.lehigh.edu/onto/univ-bench.owl>";
    std::string str2 = "http://www.w3.org/1999/02/22-rdf-syntax-ns#type";
    std::string str3 = "http://www.w3.org/2002/07/owl#Ontology";
    std::cout << token.substr(token.length()-1,1) << std::endl;
    if((token.compare(0,1,"<") == 0) && (token.compare(token.length()-1,1,">") == 0)){
        token = token.substr(1, token.length()-2);
    }
    std::cout << token << std::endl;

}