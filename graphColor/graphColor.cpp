/* Muhammet CAKIR
   29.12.2017
   Graph Coloring */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <map>
#include <list>
#include <vector>

using namespace std;

class GraphColoring{
	  int nodeNum;
	  int edgeNum;
	  int colorNum;
	  int *nodeDegree;
	  int *nodeOrder;
	  int *nodeColorNum;
	  list <int> *adjList;
	  map <int, int> *excColors;
	  multimap <int, int> degNode;
  public:
   	GraphColoring(char *);
   	void colorGraphLDO();
   	void orderNode();
   	int  findMinColor(int);
   	void printColoredGraph();
   	~GraphColoring();
};

GraphColoring::GraphColoring(char *fileName){
		ifstream file(fileName);
    if(file){
    	string line;
    	getline(file,line);
    	stringstream ss(line);
    	ss>>nodeNum>>edgeNum;

    	colorNum = -1;
    	adjList = new list<int> [nodeNum];
			excColors = new map<int,int> [nodeNum];

			nodeDegree = new int[nodeNum];
			nodeOrder = new int[nodeNum];
			nodeColorNum = new int[nodeNum];

			for(int i=0; i<nodeNum; i++){
				nodeDegree[i] = 0;
				nodeColorNum[i] = 0;
			}
			degNode.clear();

			int counter=0;
    	int first, second;
    	while(counter<edgeNum && getline(file,line)){ // reading is done line by line
      	stringstream ss(line);
        ss>>first>>second;
       	adjList[first].push_back(second);
				adjList[second].push_back(first);
				nodeDegree[first]++;
				nodeDegree[second]++;
				counter++;
      }
    }
    file.close();

    for(int i=0; i<nodeNum; i++){
    	degNode.insert(make_pair(nodeDegree[i], i));
	}
}
GraphColoring::~GraphColoring(){
	delete []nodeDegree;
	delete []nodeOrder;
	delete []nodeColorNum;
	delete []adjList;
	delete []excColors;
}
void GraphColoring::orderNode(){
	int i=0;
	int prev=0;
	int prevIndex =-1;
	multimap<int,int>::reverse_iterator iter;
	for(iter = degNode.rbegin(); iter != degNode.rend() ; iter++){
		list<int>::iterator it;
		int len=0;
		for(it = adjList[iter->second].begin(); it != adjList[iter->second].end() ; ++it){
			len += adjList[*it].size();
		}
		if(len<prev && prevIndex>=0 && nodeDegree[prevIndex] == nodeDegree[iter->second]){
			nodeOrder[i] = nodeOrder[i-1];
			nodeOrder[i-1] = iter->second;
		}else{
			nodeOrder[i] = iter->second;
		}
		i++;
		prev=len;
		prevIndex = iter->second;
	}
}

int GraphColoring::findMinColor(int node){
	map<int,int>::iterator iter;
	if(!excColors[node].empty()) {
		int color=0;
		for(iter = excColors[node].begin() ; iter!=excColors[node].end() ; iter++){
			if(iter->first != color){
				return color;
			}
			color++;
		}
		return color;
	}
	return 0;
}

void GraphColoring::colorGraphLDO(){
	int col;
	for(int i=0; i<nodeNum; i++){
		col = findMinColor(nodeOrder[i]);
		if(col > colorNum){
			colorNum = col;
		}
		nodeColorNum[nodeOrder[i]] = col;
		while(!adjList[nodeOrder[i]].empty()){
			excColors[adjList[nodeOrder[i]].back()].insert(make_pair(col,col));
			adjList[nodeOrder[i]].pop_back();
		}
	}
}
void GraphColoring::printColoredGraph(){
	cout<<colorNum+1<<endl;
	for(int i=0; i<nodeNum; i++){
		if (i != nodeNum-1){
			cout <<nodeColorNum[i]<<" ";
		}else{
			cout <<nodeColorNum[i]<<endl;
		}
	}
}
int main(int argc,char* argv[]){
	GraphColoring gc(argv[1]);
	gc.orderNode();
	gc.colorGraphLDO();
	gc.printColoredGraph();
	return 0;
}
