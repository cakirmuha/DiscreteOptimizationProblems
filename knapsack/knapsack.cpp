/* Muhammet CAKIR
   29.12.2017
   Knapsack */

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

struct Item{
	  int index;
    int value;
    int weight;
    float valuePerWeight;
};

struct Node{
    int level;
    int value;
    int weight;
    float bound;		    // for upper bound
    vector<int>decision;	//hold optimal solution state
    bool operator<(const Node &rhs) const{ // for priority queue
        return (bound < rhs.bound);
    }
};

class Knapsack{
		int itemCount;
		int capacity;
		int maxProfit;
		Item *sortedItemList;
		int *optSol;				  // optimal solution of read items
		vector <int> optimalSolution; //optimal solution of sorted items
  public:
   	Knapsack(char *);
   	void bestFirstBB();
   	float bound(Node);
   	void printKnapsack();
   	~Knapsack();
};

bool cmpValPerWeight (Item one, Item two) { return one.valuePerWeight > two.valuePerWeight;}

Knapsack::Knapsack(char *fileName){
	maxProfit = 0;
	ifstream file(fileName);
    if(file){
    	string line;
    	// first line <item number, capacity>
    	getline(file,line);
    	stringstream ss(line);
    	ss>>itemCount>>capacity;

    	int counter = 0;
    	sortedItemList = new Item[itemCount];
    	optSol = new int[itemCount];
    	int first, second;
    	while(counter < itemCount && getline(file,line)){ // reading is done line by line
              stringstream ss(line);
              ss>>first>>second;
              sortedItemList[counter].value = first;
              sortedItemList[counter].weight = second;
              counter++;
        }
        for (int i=0; i<itemCount; i++){
        	sortedItemList[i].valuePerWeight = (float)sortedItemList[i].value/(float)sortedItemList[i].weight;
        	sortedItemList[i].index = i;
        	optSol[i] =0 ;
    	}
    }
    // sort items by ValuePerWeight
    sort(sortedItemList, sortedItemList+itemCount, cmpValPerWeight);
}
Knapsack::~Knapsack(){
	delete []optSol;
	delete []sortedItemList;
}
void Knapsack::printKnapsack(){
	for(int i=0; i<itemCount; i++){
		optSol[sortedItemList[i].index] = optimalSolution.at(i);
	}
	cout<<maxProfit<<endl;
	for(int i=0; i<itemCount-1; i++){
		cout<<optSol[i]<<" ";
	}
	cout<<optSol[itemCount-1]<<endl;
}

void Knapsack::bestFirstBB() {
    priority_queue<Node>Q;
    Node u, v; // v as root

    v.level = 0;
	  v.value = 0;
	  v.weight = 0;
	  v.decision.resize(itemCount, 0);
	  u.decision.resize(itemCount,0);

    v.bound = bound(v);
    Q.push(v);

    while(!Q.empty()){
        v= Q.top();
        Q.pop(); // best bound node is removed
        if(v.bound > maxProfit) { //expand v
            u.level = v.level + 1;  // u = child of v
            u.weight = v.weight + sortedItemList[u.level-1].weight;
            u.value = v.value + sortedItemList[u.level-1].value;
            u.decision = v.decision;
            u.decision.at(u.level-1) = 1;
            if(u.weight <= capacity && u.value > maxProfit) {
                maxProfit = u.value;
                optimalSolution = u.decision;
            }

            u.bound = bound(u); // upper bound of child u
            if(u.bound > maxProfit && u.weight < capacity) {
                Q.push(u);
            }

            u.weight = v.weight;
            u.value = v.value;
            u.decision.at(u.level-1) = 0;
            u.bound = bound(u); // chose node to expand
            if(u.bound > maxProfit && u.weight < capacity) {
                Q.push(u);
            }
        }
    }
}

float Knapsack::bound(Node u) {
	if(u.weight >= capacity)
        return 0;
    int totalWeight;
    float bound;
    bound = (float)u.value;
    int j = u.level;
    totalWeight = u.weight;
    while(j < itemCount && totalWeight + sortedItemList[j].weight <= capacity){
        totalWeight += sortedItemList[j].weight;
        bound += sortedItemList[j].value;
        j++;
    }
    int k=j;
    if(k<itemCount) // fraction of item
        bound +=  (capacity - totalWeight) * sortedItemList[k].valuePerWeight;
    return bound;
}

int main(int argc, char *argv[]) {
	Knapsack ks(argv[1]);
	ks.bestFirstBB();
	ks.printKnapsack();
}
