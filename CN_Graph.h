#ifndef CN_Graph
#define CN_Graph

#include "CN_Node.h"
#include "CN_AtomicEdge.h"
#include "CN_SingleCreditEdge.h"
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

	Graph();
	Graph(int nodeNum);
	~Graph();

	/////////////////////////////////////////////////////////////////////////
	/* Generate Initial Network */
	/////////////////////////////////////////////////////////////////////////
	void generateTestGraph2();
	void generateTestGraph3();
	void genTest0Graph(double threshold, int numIR, int cap);
	void genTest1Graph(double threshold, int numIR, int cap);

};


#endif
