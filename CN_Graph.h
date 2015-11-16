#ifndef CN_Graph
#define CN_Graph

#include "CN_Node.h"
#include <list>
#include <unordered_map>

class Graph{
public:
	int nodeNum;
	unordered_map<int, Node*> nodes;
	unordered_map<int, AtomicEdge*> atomicEdges;
	

	/////////////////////////////////////////////////////////////////////////
	/* Graph basics */
	/////////////////////////////////////////////////////////////////////////
	Graph();
	Graph(int nodeNum);
	Graph(Graph &graphT);
	Graph& operator=(Graph &graphT);
	~Graph();

	void print();
	
	void addMultiEdge(Node* nodeFrom, Node* nodeTo, double ir, int currDebt, int cap);

	void setRoutePreference(int opMode, vector<string> &v);
	/////////////////////////////////////////////////////////////////////////
	/* Generate Initial Network */
	/////////////////////////////////////////////////////////////////////////
	void generateTestGraph();
	void generateTestGraph2();
	void genTest0Graph(double threshold, int numIR);

};


#endif