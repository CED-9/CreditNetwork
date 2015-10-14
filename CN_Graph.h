#ifndef CN_Graph
#define CN_Graph

#include "CN_Node.h"
#include <list>
#include <unordered_map>

class Graph{
public:
	int nodeNum;
	unordered_map<int, Node*> nodes;

	/////////////////////////////////////////////////////////////////////////
	/* Graph basics */
	/////////////////////////////////////////////////////////////////////////
	Graph();
	Graph(int nodeNum);
    Graph(Graph &graphT);
    Graph& operator=(Graph &graphT);
    ~Graph();

	void print();
	Node* searchID(int id);
	
	void addUnitEdge(Node* node1, Node* node2, double ir, int currDebt);

	/////////////////////////////////////////////////////////////////////////
	/* Generate Initial Network */
	/////////////////////////////////////////////////////////////////////////
	void generateTestGraph();

};


#endif