
#include "CN_Edge.h"
#include <iostream>

using namespace std;

Edge::Edge(Node* nodeFromT, Node* nodeToT){
	this->nodeFrom = nodeFromT;
	this->nodeTo = nodeToT;
}

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

void Edge::addUnitEdge(int cap, int current, double interest_rate){

	// cout << "hehe: " << cap << " " << current << " " << interest_rate << endl;

	for (int i = 0; i < unitEdges.size(); ++i){
		if (unitEdges[i].credit_interest_rate == interest_rate){
			// cout << "another time " << cap << " " << current << endl;
			unitEdges[i].c_max += cap;
			unitEdges[i].d_current += current;
			return;
		}
	}

// cout << "first time" << endl;

	UnitEdge temp;
	temp.c_max = cap;
	temp.d_current = current;
	temp.credit_interest_rate = interest_rate;
	temp.debt_interest_rate = interest_rate;

	unitEdges.push_back(temp);
}

Edge::~Edge(){}


////////////////////////////////////////////////////////////////////
// one unit edge
////////////////////////////////////////////////////////////////////
void Edge::setCurrent(int d){
	unitEdges[0].d_current = d;
}

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
	cout << "From " << nodeFrom->getNodeId() << " To " << nodeTo->getNodeId() << endl;

	for (int i = 0; i < unitEdges.size(); ++i){
		cout << " ... credit IR " << unitEdges[i].credit_interest_rate
		<< " debt IR " << unitEdges[i].debt_interest_rate
		<< " Credit " << unitEdges[i].c_max
		<< " Debt " << unitEdges[i].d_current;
		cout << endl;
	}
}