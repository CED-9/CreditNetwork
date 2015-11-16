// Node.C

#include "CN_Node.h"
#include <queue>
#include <vector>
#include <fstream>

Node::Node(int id){
	this->nodeId = id;
	this->transactionNum = 0;
	this->totalIR = 0;
	this->routePreference = "";
}

Node::Node(const Node& n){
	this->nodeId = n.nodeId;
	this->transactionNum = n.transactionNum;
	this->totalIR = n.totalIR;
	this->routePreference = n.routePreference;
	for (auto it : edge_in){
		pair<int, Edge*> p;
		p.first = it.first;
		p.second = new Edge(*(it.second));
		this->edge_in.insert(p);
	}
	for (auto it : edge_out){
		pair<int, Edge*> p;
		p.first = it.first;
		p.second = new Edge(*(it.second));
		this->edge_in.insert(p);
	}
}

Node::~Node(){
	for (auto it : edge_in){
		delete it.second;
	}
}

void Node::print(){
	cout << "Node " << this->nodeId << endl;
	for (auto it : edge_in){
		it.second->print();
	}
	for (auto it : edge_out){
		it.second->print();
	}
	cout << endl;
	return;
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