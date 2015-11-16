
#include "CN_Edge.h"
#include <iostream>

using namespace std;

Edge::Edge(Node* nodeFromT, Node* nodeToT){
	this->nodeFrom = nodeFromT;
	this->nodeTo = nodeToT;
}

Edge::Edge(const Edge& e, Node* nodeFromT, Node* nodeToT, 
	unordered_map<int, AtomicEdge*>& atomicMap){
	this->nodeFrom = nodeFromT;
	this->nodeTo = nodeToT;
	for (int i = 0; i < e.singleCreditEdges.size(); ++i){
		SingleCreditEdge* s = new SingleCreditEdge(*(e.singleCreditEdges[i]), 
			this, this->singleCreditEdges.size(), atomicMap);
		this->singleCreditEdges.push_back(s);
	}
}

Edge::~Edge(){
	for (int i = 0; i < singleCreditEdges.size(); ++i){
		delete singleCreditEdges[i];
	}
}

SingleCreditEdge* Edge::addSingleCreditEdge(double interest_rate, int cap, 
	int& atomicGlobalId, unordered_map<int, AtomicEdge*>& atomicMap){

	SingleCreditEdge* temp = new SingleCreditEdge(cap, 
		interest_rate, atomicGlobalId, this, 
		this->singleCreditEdges.size(), atomicMap);
	this->singleCreditEdges.push_back(temp);
	return temp;
	
}

void Edge::routeAtomicEdge(AtomicEdge* a, int flow, double interest_rate, 
	int& atomicGlobalId, unordered_map<int, AtomicEdge*>& atomicMap){

	if (a->isDebt){
		a->originEdge->singleCreditEdges[a->singleCreditIndex]
			->routeDebt(flow, a->interest_rate);
	} else {
		a->originEdge->singleCreditEdges[a->singleCreditIndex]
			->routeCredit(flow, interest_rate, atomicGlobalId, 
				this, a->singleCreditIndex, atomicMap);
	}

}

void Edge::print(){
	cout << "From Node " << nodeFrom->getNodeId() 
	<< " To Node " << nodeTo->getNodeId() << endl;

	for (int i = 0; i < singleCreditEdges.size(); ++i){
		singleCreditEdges[i]->print();
	}
	
}