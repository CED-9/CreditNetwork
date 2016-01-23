#ifndef CN_Graph
#define CN_Graph

#include "CN_Node.h"
#include "CN_Edge.h"
#include <list>
#include <unordered_map>
#include <fstream>
#include <set>


class Graph{
public:
	int nodeNum;
	int atomicGlobalId;
	unordered_map<int, Node*> nodes;
	list<Edge*> edges;
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
	void printAtomicEdges();
	void printAtomicIouEdges(ofstream& fout);
	void printAvgAtomicIouEdges();
	
	void addMultiEdge(Node* nodeFrom, Node* nodeTo, double credit_ir, double debt_ir, int currDebt, int cap);

	void setRoutePreference(int opMode, vector<string> &v);
	/////////////////////////////////////////////////////////////////////////
	/* Generate Initial Network */
	/////////////////////////////////////////////////////////////////////////
	void generateTestGraph2();
	void generateTestGraph3();
	void genTest0Graph(double threshold, int numIR, int cap);

};


#endif
