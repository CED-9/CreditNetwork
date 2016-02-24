#ifndef CN_CplexConverter
#define CN_CplexConverter

#include "CN_Constants.h"
#include "CN_Node.h"
#include "CN_Graph.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <set>

class CplexConverter{

public:
	int transSeqNum;

	struct Variable{
		int varId;
		int atomicEdgeId;
		double interest_rate;
		Variable(int v, int a, double ir) : varId(v), atomicEdgeId(a), interest_rate(ir) {}
	};

	vector<Variable> variables;
	unordered_map < int, vector<int> > atomicIdToVarIdDict;
	unordered_map < int, int > varIdToAtomicIdDict;
	vector<double> capacities;
	vector<double> results;
	double request;
	Node* src;
	Node* dest;
	Graph* graph;
	
	void constructCplex(Graph* g, Node* s, Node* t, int req, int transSeqNumT);

	void printInput();

	void printResult();

	void copyBack();
};


#endif
