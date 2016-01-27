// Node.C

#include "CN_Node.h"
#include <queue>
#include <vector>
#include <fstream>

extern void printEdge(Edge*);
extern void printAtomicEdge(AtomicEdge*);

Node::Node(int id){
	this->nodeId = id;
	this->transactionNum = 0;
	this->totalIR = 0;
	this->routePreference = "";
}


Node::~Node(){
	// for (auto it : edge_in){
	// 	delete it.second;
	// }
}

void Node::print(){
	cout << "Node " << this->nodeId << endl;
	for (auto &it : edge_in){
		printEdge(it.second);
	}
	for (auto &it : edge_out){
		printEdge(it.second);
	}
	cout << "Atomic Edges: " << endl;
	for (auto &it : atomicEdge_in){
		printAtomicEdge(it.second);
	}
	for (auto &it : atomicEdge_out){
		printAtomicEdge(it.second);
	}
	cout << endl;

	return;
}

void Node::updateDegree(){
	this->degree = edge_in.size();
}

int Node::getNodeId(){ return nodeId; }

double Node::getCurrBanlance(){
	double temp = 0;
	// for (auto it : edge_in){
	// 	temp -= it.second->get_d_current() * it.second->get_interest_rate();
	// }
	// for (auto it : edge_out){
	// 	temp += it.second->get_d_current() * it.second->get_interest_rate();
	// }
	return temp;
}