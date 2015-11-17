
#include "CN_Graph.h"
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

/////////////////////////////////////////////////////////////////////////
/* Graph basics */
/////////////////////////////////////////////////////////////////////////
Graph::Graph(){
	this->atomicGlobalId = 0;
}

Graph::Graph(int nodeNumT){
	this->nodeNum = nodeNumT;
	this->atomicGlobalId = 0;
	for (int i = 0; i < nodeNum; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}
}

static void deepCopyHelper(Graph* newGraph, Graph& oldGraph){

	newGraph->nodeNum = oldGraph.nodeNum;
	newGraph->atomicGlobalId = oldGraph.atomicGlobalId;
	for (auto& it : oldGraph.nodes){
		newGraph->nodes[it.first] = new Node(it.first);
		newGraph->nodes[it.first]->transactionNum = it.second->transactionNum;
		newGraph->nodes[it.first]->totalIR = it.second->totalIR;
		newGraph->nodes[it.first]->routePreference = it.second->routePreference;
	}
	for (auto& oldNodePair : oldGraph.nodes){
		for (auto& oldEdgePair : oldNodePair.second->edge_in){
			int fromId = oldEdgePair.first;
			int toId = oldNodePair.first;
			Node* newFromNode = newGraph->nodes[fromId];
			Node* newToNode = newGraph->nodes[toId];

			Edge* e = new Edge(*(oldEdgePair.second),
				newFromNode, newToNode, newGraph->atomicEdges);
			newToNode->edge_in[fromId] = e;
			newFromNode->edge_out[toId] = e;
		}
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


void Graph::addMultiEdge(Node* nodeFrom, Node* nodeTo, 
	double credit_ir, double debt_ir, int currDebt, int cap){

	// use diff ir to create ious
	Edge* e;
	if (nodeTo->edge_in.find(nodeFrom->getNodeId()) == nodeTo->edge_in.end()){
		e = new Edge(nodeFrom, nodeTo);
		nodeTo->edge_in[nodeFrom->getNodeId()] = e;
		nodeFrom->edge_out[nodeTo->getNodeId()] = e;
	} else {
		e = nodeTo->edge_in[nodeFrom->getNodeId()];
	}

	SingleCreditEdge* temp = e->addSingleCreditEdge(credit_ir, cap, this->atomicGlobalId, this->atomicEdges);
	if (currDebt != 0){
		e->routeAtomicEdge(temp->credit_remain, currDebt, debt_ir, atomicGlobalId, this->atomicEdges);
	}

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
					// this->addUnitEdge(nodes.find(i)->second, nodes.find(j)->second, ir, rand()%2);
				} else {
					// this->addUnitEdge(nodes.find(j)->second, nodes.find(i)->second, ir, rand()%2);
				}
			}

		}
	}
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
	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.2, 0.4, 2, 2);

	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.3, 0.3, 3, 3);

	this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.3, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(2)->second, nodes.find(4)->second, 0.2, 0.2, 4, 4);

	this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.2, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.4, 0.0, 0, 1);

	this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.3, 0.0, 0, 3);

	this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.4, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(6)->second, nodes.find(1)->second, 0.2, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(7)->second, nodes.find(6)->second, 0.3, 0.0, 0, 1);

	this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.2, 0.0, 0, 2);

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