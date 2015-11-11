#ifndef CN_Graph
#define CN_Graph

#include "CN_Node.h"
#include <list>
#include <unordered_map>

class Graph{
public:
	int nodeNum;
	unordered_map<int, Node*> nodes;
	vector<Edge*> nodes;

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