/* Muhammet CAKIR
   29.12.2017
   Vehicle Routing */

#include <iostream>
#include <queue>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

struct Node {
	int index;
	int demand;
	float x, y;
	bool isUsed;
};
class Vehicle{
    int capacity;
    int load;
    int currentCustomer; // location of vehicle
    vector<Node>route;
  public:
  	Vehicle(){};
    Vehicle(int);
    int getCurrentCustomer(){ return currentCustomer;}
		vector<Node> getRoute(){ return route;}
    void addNode(Node);
    bool hasCapViolation(int);

};
Vehicle::Vehicle(int capParam){
	capacity = capParam;
	load = 0;
    currentCustomer = 0;
}
void Vehicle::addNode(Node Custom){
	route.push_back(Custom);
    load = load + Custom.demand;
    currentCustomer = Custom.index;
}
bool Vehicle::hasCapViolation(int demand){
	return (demand + load > capacity);
}

class VehicleRouting{
		int N;
		int V;
		int C;
		float Obj;
		Vehicle *Vehicles;
		Node *Nodes;
		float **distMatrix;
  public:
   	VehicleRouting(char *);
   	float calculateDist(int,int);
   	bool hasNotRoutedCustomer();
   	void greedySolution();
   	void printSolution();

};
float VehicleRouting::calculateDist(int i, int j){
	float first = pow((Nodes[i].x-Nodes[j].x), 2);
	float second = pow((Nodes[i].y-Nodes[j].y), 2);
	float dist = sqrt(first+second);
	return dist;
}
VehicleRouting::VehicleRouting(char *fileName){
		ifstream file(fileName);
    if(file){
    	string line;
    	// first line <customers,vehicles,capacity>
    	getline(file,line);
    	stringstream ss(line);
    	ss>>N>>V>>C;

    	int counter = 0;
			Obj = 0;
			Nodes = new Node[N];
			Vehicles = new Vehicle[V];
    	int demand;
			float x,y;
    	while(counter < N && getline(file,line)){ // reading is done line by line
              stringstream ss(line);
              ss>>demand>>x>>y;
              Nodes[counter].demand = demand;
              Nodes[counter].x = x;
              Nodes[counter].y = y;
              Nodes[counter].index = counter;
              counter++;
      }
      distMatrix = new float*[N];
			for(int i = 0; i < N; i++)
				distMatrix[i] = new float[N];

			for(int i = 0; i < N; i++){
				distMatrix[i][i] = 0;
				for(int j = i+1; j < N; j++){
					float distance = calculateDist(i,j);
					distMatrix[i][j] = distance;
					distMatrix[j][i] = distance;
				}
			}

			for(int i=0; i<V; i++){
				Vehicles[i] = Vehicle(C);
			}
    }
}

bool VehicleRouting::hasNotRoutedCustomer(){
	for(int i=1; i<N; i++){
        if (!Nodes[i].isUsed){
        	return true;
				}
  }
  return false;
}

void VehicleRouting::greedySolution(){
	float finalCost;
	int vehicleIndex =0;
	int customerIndex = 0;
	while(hasNotRoutedCustomer()){
		customerIndex = 0;
		Node candidateCustomer;
		bool candAssigned = false;
		float minCost = numeric_limits<float>::max();
		if (Vehicles[vehicleIndex].getRoute().empty())
			Vehicles[vehicleIndex].addNode(Nodes[0]);
		for(int i=1; i<N; i++){
			if (!Nodes[i].isUsed){
				if(!Vehicles[vehicleIndex].hasCapViolation(Nodes[i].demand)){
					if(minCost > distMatrix[Vehicles[vehicleIndex].getCurrentCustomer()][i]){
						minCost = distMatrix[Vehicles[vehicleIndex].getCurrentCustomer()][i];
						customerIndex = i;
						candidateCustomer = Nodes[i];
						candAssigned= true;
					}
				}
			}
		}
		if(candAssigned){ // a customer was found
				Vehicles[vehicleIndex].addNode(candidateCustomer);
        Nodes[customerIndex].isUsed = true;
        Obj = Obj + minCost;
		}else{
			if (vehicleIndex+1 >= V){
				cout<<"NO FEASIBLE SOLUTION, SOME CUSTOMERS ARE NOT VISITED"<<endl;
				break;
			}
			if(Vehicles[vehicleIndex].getCurrentCustomer() != 0){
				finalCost = distMatrix[Vehicles[vehicleIndex].getCurrentCustomer()][0];
				Vehicles[vehicleIndex].addNode(Nodes[0]);
				Obj = Obj + finalCost;
			}
			vehicleIndex += 1;
		}
	}

	finalCost = distMatrix[Vehicles[vehicleIndex].getCurrentCustomer()][0];
    Vehicles[vehicleIndex].addNode(Nodes[0]);
    Obj = Obj + finalCost;
}

void VehicleRouting::printSolution(){
	cout<<setprecision(1)<<fixed;
	cout<<Obj<<endl;
	for(int i=0; i<V; i++){
		if(!Vehicles[i].getRoute().empty()){
			int routeSize = Vehicles[i].getRoute().size();
			for (int j = 0; j<routeSize ; j++) {
            if (j == routeSize-1){
                cout<<Vehicles[i].getRoute()[j].index<<endl;
            }else{
							cout<<Vehicles[i].getRoute()[j].index<<" ";
						}
      }
		}else{
			cout<<0<<" "<<0<<endl;
		}
	}
}

int main(int argc, char *argv[]) {
	VehicleRouting vrp(argv[1]);
	vrp.greedySolution();
	vrp.printSolution();
	return 0;
}
