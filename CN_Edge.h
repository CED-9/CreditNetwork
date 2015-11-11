#ifndef CN_Edge
#define CN_Edge

#include "CN_Node.h"
#include <vector>
#include <iostream>

using namespace std;

class Node;

struct UnitEdge{
	int c_max;
	int d_current;
	double credit_interest_rate; // steady
	double debt_interest_rate;
};

class Edge{

public:
	Node* nodeFrom;
	Node* nodeTo;
	vector <UnitEdge> unitEdges;

	Edge(Node* nodeFromT, Node* nodeToT);
	Edge(Node* nodeFromT, Node* nodeToT, double interest_rate);
	~Edge();

	void addUnitEdge(int cap, int current, double interest_rate);

	////////////////////////////////////////////////////////////////////
	// one unit edge
	////////////////////////////////////////////////////////////////////

	void setCurrent(int d);

	int get_c_remain();

	int get_d_current();

	void set_interest_rate(double interest_rateT);

	void set_debt_interest_rate(double interest_rateT);

	double get_interest_rate();

	double get_debt_interest_rate();

	void print();

};



#endif