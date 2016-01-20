#ifndef CN_CplexConverter
#define CN_CplexConverter

#include "CN_WidgetGraph.h"
#include "CN_Constants.h"


class CplexConverter{

public:

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
	
	void constructCplex(Graph* g, Node* s, Node* t, int req);

	void printInput();

	void printResult();

	void copyBack();
};


#endif