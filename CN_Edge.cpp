
#include "CN_Edge.h"
#include <iostream>

using namespace std;

Edge::Edge(Node* nodeFromT, Node* nodeToT, double interest_rate){
	UnitEdge temp;
	temp.c_max = 1;
	temp.d_current = 0;
	temp.credit_interest_rate = interest_rate;
	temp.debt_interest_rate = interest_rate;

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
	unitEdges[0].credit_interest_rate = interest_rateT;
	unitEdges[0].debt_interest_rate = interest_rateT;
	return;
}

double Edge::get_interest_rate(){
	return unitEdges[0].credit_interest_rate;
}

void Edge::set_debt_interest_rate(double interest_rateT){
	unitEdges[0].debt_interest_rate = interest_rateT;
}

double Edge::get_debt_interest_rate(){
	return unitEdges[0].debt_interest_rate;
}

void Edge::print(){
	cout << "From " << nodeFrom->getNodeId() << " To " << nodeTo->getNodeId();
	cout << " ... credit IR " << unitEdges[0].credit_interest_rate
	<< " debt IR " << unitEdges[0].debt_interest_rate
	<< " Credit " << unitEdges[0].c_max
	<< " Debt " << unitEdges[0].d_current;
	cout << endl;
}