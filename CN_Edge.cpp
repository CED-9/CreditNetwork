
#include "CN_Edge.h"
#include <iostream>

using namespace std;




// SingleCreditEdge* Edge::addSingleCreditEdge(double interest_rate, int cap, 
// 	int& atomicGlobalId, unordered_map<int, AtomicEdge*>& atomicMap){

// 	SingleCreditEdge* temp = new SingleCreditEdge(cap, 
// 		interest_rate, atomicGlobalId, this, 
// 		this->singleCreditEdges.size(), atomicMap, nodeFrom, nodeTo);
// 	this->singleCreditEdges.push_back(temp);
// 	return temp;
	
// }

// void Edge::routeAtomicEdge(AtomicEdge* a, int flow, double interest_rate, 
// 	int& atomicGlobalId, unordered_map<int, AtomicEdge*>& atomicMap, int transSeqNum){

// 	if (a->isDebt){
// 		a->originEdge->singleCreditEdges[a->singleCreditIndex]
// 			->routeDebt(flow, a->interest_rate);
// 	} else {
// 		a->originEdge->singleCreditEdges[a->singleCreditIndex]
// 			->routeCredit(flow, interest_rate, atomicGlobalId, 
// 				this, a->singleCreditIndex, atomicMap, nodeFrom, nodeTo);
// 	}

// 	a->nodeTo->addModification(transSeqNum);
// 	this->edgeUsage.push_back(transSeqNum);

// }

