#ifndef CN_Graph
#define CN_Graph

#include "CN_Node.h"
#include <list>
#include <unordered_map>
#include <set>


class Graph{
public:
	int nodeNum;
	int atomicGlobalId;
	unordered_map<int, Node*> nodes;
	unordered_map<int, AtomicEdge*> atomicEdges;
	set<doulbe> allInterests;
	

	/////////////////////////////////////////////////////////////////////////
	/* Graph basics */
	/////////////////////////////////////////////////////////////////////////
	Graph();
	Graph(int nodeNum);
	Graph(Graph &graphT);
	Graph& operator=(Graph &graphT);
	~Graph();

	void print();
	
	void addMultiEdge(Node* nodeFrom, Node* nodeTo, double credit_ir, double debt_ir, int currDebt, int cap);

	void setRoutePreference(int opMode, vector<string> &v);
	/////////////////////////////////////////////////////////////////////////
	/* Generate Initial Network */
	/////////////////////////////////////////////////////////////////////////
	void generateTestGraph2();
	void genTest0Graph(double threshold, int numIR);

};


#endif