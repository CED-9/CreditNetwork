
#include "CN_Edge.h"
#include <iostream>

using namespace std;

void Edge::Edge(Node* nodeFromT, Node* nodeToT, double interest_rate){
	UnitEdge temp;
	temp.c_max = 1;
	temp.d_current = 0;
	temp.interest_rate = interest_rate;
	unitEdges.push_back(temp);
	this->nodeFrom = nodeFromT;
	this->nodeTo = nodeToT;
}

void Edge::setCurrent(int d){
	unitEdges[0].d_current = d;
}

Edge::~Edge(){}

int Edge::get_c_remain(){
	return unitEdges[0].c_max - unitEdges[0].d_current;
}

int Edge::get_d_current(){
	return unitEdges[0].d_current;
}

void Edge::set_interest_rate(double interest_rateT){
	unitEdges[0].interest_rate = interest_rateT;
	return;
}

double Edge::get_interest_rate(){
	return unitEdges[0].interest_rate;
}

void Edge::print(){
	cout << "From " << nodeFrom->getNodeId() << " To " << nodeTo->getNodeId();
	cout << " ... IR " << unitEdges[0].interest_rate
	<< " Credit " << unitEdges[0].c_max
	<< " Debt " << unitEdges[0].d_current;
	cout << endl;
}