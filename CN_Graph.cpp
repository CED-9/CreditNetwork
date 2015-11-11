
#include "CN_Graph.h"
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

/////////////////////////////////////////////////////////////////////////
/* Graph basics */
/////////////////////////////////////////////////////////////////////////
Graph::Graph(){}

Graph::Graph(int nodeNumT){
	this->nodeNum = nodeNumT;
	for (int i = 0; i < nodeNum; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}
}

static void deepCopyHelper(Graph* newGraph, Graph& oldGraph){
	for (auto& it : oldGraph.nodes){
		std::pair<int, Node*> tempPair;
		tempPair.first = it.first;
		tempPair.second = it.second;
		newGraph->nodes.insert(tempPair);
	}
}

Graph::Graph(Graph &graphT){
	deepCopyHelper(this, graphT);
}

Graph& Graph::operator=(Graph &graphT){
	deepCopyHelper(this, graphT);
	return *this;
}

Graph::~Graph(){
	for (auto& it : nodes){
		delete it.second;
	}
}


void Graph::addMultiEdge(Node* nodeFrom, Node* nodeTo, double ir, int currDebt, int cap){

	if (nodeTo->edge_in.find(nodeFrom->getNodeId()) == nodeTo->edge_in.end()){
		// cout << "not found!" << endl;
		std::pair<int, Edge*> edge;
		edge.first = nodeFrom->getNodeId();
		edge.second = new Edge(nodeFrom, nodeTo);
		edge.second->addUnitEdge(cap, currDebt, ir);
		nodeTo->edge_in.insert(edge);
		edge.first = nodeTo->getNodeId();
		nodeFrom->edge_out.insert(edge);
		return;
	}

	// cout << "found!" << endl;
	nodeTo->edge_in.find(nodeFrom->getNodeId())->second->addUnitEdge(cap, currDebt, ir);
}

void Graph::addUnitEdge(Node* nodeFrom, Node* nodeTo, double ir, int currDebt){

	std::pair<int, Edge*> edge;

	edge.first = nodeFrom->getNodeId();
	edge.second = new Edge(nodeFrom, nodeTo);
	edge.second->addUnitEdge(1, currDebt, ir);

	nodeTo->edge_in.insert(edge);
	edge.first = nodeTo->getNodeId();
	nodeFrom->edge_out.insert(edge);

}

void Graph::print(){
	for (auto& it : nodes){
		it.second->print();
	}
}

/////////////////////////////////////////////////////////////////////////
/* Generate Initial Network */
/////////////////////////////////////////////////////////////////////////
void Graph::genTest0Graph(double threshold, int numIR){

	default_random_engine generator;
	uniform_real_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < nodes.size(); i++){
		for (int j = i+1; j < nodes.size(); j++){

			double num = distribution(generator);
			double ir = (rand() % numIR + 1)/100.0;
			if (num > 1.0 - threshold){
				if (rand()%2 == 1) {
					this->addUnitEdge(nodes.find(i)->second, nodes.find(j)->second, ir, rand()%2);
				} else {
					this->addUnitEdge(nodes.find(j)->second, nodes.find(i)->second, ir, rand()%2);
				}
			}

		}
	}
}

void Graph::generateTestGraph(){

	nodeNum = 6;
	for (int i = 0; i < 6; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}
	
	this->addUnitEdge(nodes.find(0)->second, nodes.find(1)->second, 0.3, 1);
	this->addUnitEdge(nodes.find(1)->second, nodes.find(2)->second, 0.2, 0);
	this->addUnitEdge(nodes.find(3)->second, nodes.find(1)->second, 0.2, 0);
	this->addUnitEdge(nodes.find(4)->second, nodes.find(1)->second, 0.4, 0);
	this->addUnitEdge(nodes.find(2)->second, nodes.find(3)->second, 0.1, 0);
	this->addUnitEdge(nodes.find(3)->second, nodes.find(4)->second, 0.1, 0);
	this->addUnitEdge(nodes.find(5)->second, nodes.find(4)->second, 0.3, 0);
	this->addUnitEdge(nodes.find(5)->second, nodes.find(3)->second, 0.4, 0);
	this->addUnitEdge(nodes.find(5)->second, nodes.find(2)->second, 0.2, 0);
}

void Graph::generateTestGraph2(){

	nodeNum = 8;
	for (int i = 0; i < 8; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}

	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.3, 1, 10);
	this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.3, 0, 3);
	this->addMultiEdge(nodes.find(2)->second, nodes.find(4)->second, 0.2, 1, 6);
	this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.2, 0, 7);
	this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.4, 0, 1);
	this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.2, 0, 3);
	this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.4, 0, 7);
	this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.3, 0, 4);
	this->addMultiEdge(nodes.find(6)->second, nodes.find(1)->second, 0.2, 0, 2);
	this->addMultiEdge(nodes.find(7)->second, nodes.find(6)->second, 0.3, 0, 1);

	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.3, 1, 10);
	this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.3, 0, 3);
	this->addMultiEdge(nodes.find(2)->second, nodes.find(4)->second, 0.2, 1, 6);
	this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.2, 0, 7);
	this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.4, 0, 1);
	this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.2, 0, 3);
	this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.4, 0, 7);
	this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.3, 0, 4);
	this->addMultiEdge(nodes.find(6)->second, nodes.find(1)->second, 0.2, 0, 2);
	this->addMultiEdge(nodes.find(7)->second, nodes.find(6)->second, 0.3, 0, 1);

	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.6, 1, 10);
	this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.6, 0, 3);
	this->addMultiEdge(nodes.find(2)->second, nodes.find(4)->second, 0.4, 1, 6);
	this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.4, 0, 7);
	this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.8, 0, 1);
	this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.4, 0, 3);
	this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.8, 0, 7);
	this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.6, 0, 4);
	this->addMultiEdge(nodes.find(6)->second, nodes.find(1)->second, 0.4, 0, 2);
	this->addMultiEdge(nodes.find(7)->second, nodes.find(6)->second, 0.6, 0, 1);
}

void Graph::setRoutePreference(int opMode, vector<string> &v){
	// cout<<opMode<<endl;
	if (opMode == 1){
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "FF";
		}
	} else if (opMode == 2){
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "LP_SOURCE";
		}
	} else if (opMode == 3){        
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "LP_OVERALL";
		}
	}
	else if (opMode == 4){  
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "1";
			// not done 
		}
	}
	else if (opMode == 5){        
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = v[i];
		}       
	}
	else if (opMode == 6){
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "LP_MIN";
		}
	}
	else if (opMode == 7){
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "LP_MAX";
		}
	}
	else if (opMode == 8){
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "LP_SHORT";
		}
	}
	else if (opMode == 9){
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "BFS_SHORT";
		}
	}
	else if (opMode == 10){
		for (int i = 0; i < nodeNum; i++) {
			nodes.find(i)->second->routePreference = "BFS_LOW_IR";
		}
	}
	return;
}