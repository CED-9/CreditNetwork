#ifndef CN_Edge
#define CN_Edge

#include "CN_Node.h"
#include <vector>
#include <iostream>

using namespace std;

class Node;

// atomic edge, 0 flow, cappacity
struct AtomicEdge{
	bool isDebt;
	int atomicEdgeId;
	int cap;
	double interest_rate;
	AtomicEdge(bool d, int id, int capT, double ir)
		: isDebt(d), atomicEdgeId(id), cap(capT), interest_rate(ir) {}
};


class SingleCreditEdge{
public:
	int singleCreditEdgeId;
	int credit_max;
	double credit_interest_rate;

	AtomicEdge* credit_remain;
	unordered_map<double, AtomicEdge*> debt_current;

	SingleCreditEdge(int c_max, double ir, int& globalId) : 
		credit_max(c_max), interest_rate(ir){
			credit_remain = new AtomicEdge(0, globalId++, c_max, ir);
	}
	~SingleCreditEdge(){
		delete credit_remain;
		for (auto it : debt_current){
			delete it.second;
		}
	}

	bool addDebtEdge(int current, double interest_rate, int& globalId){
		if (credit_remain->cap < current){
			return false;
		}

		if (debt_current.find(interest_rate) != debt_current.end()){
			debt_current.find(interest_rate)->second.cap += current;
		} else {
			AtomicEdge* temp = new AtomicEdge(1, globalId++, current, ir);
			std::pair<double, AtomicEdge*> p;
			p.first = ir;
			p.second = temp;
			debt_current.insert(p);
		}
		credit_remain->cap -= current;
		return true;
	}

	bool subDebtEdge(int current, double interest_rate, int& globalId) {
		if (debt_current.find(interest_rate)->second.cap < current){
			return false;
		}
		debt_current.find(interest_rate)->second.cap -= current;
		credit_remain->cap += current;
		return true;
	}
};

struct UnitEdge{
	int unitEdgeId;
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
	vector <SingleCreditEdge> singleCreditEdges;

	Edge(Node* nodeFromT, Node* nodeToT);
	Edge(Node* nodeFromT, Node* nodeToT, double interest_rate);
	~Edge();

	void addUnitEdge(int cap, int current, double interest_rate);
	void addSingleCreditEdge(int cap, int current, double interest_rate);

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