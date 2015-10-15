#ifndef CN_Node
#define CN_Node

#include "CN_Edge.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// forward declaration
class Graph;
class Edge;

class Node{

public:
	int nodeId;
	int transactionNum;
	string routePreference;

	// edges
	unordered_map<int, Edge*> edge_out;
	unordered_map<int, Edge*> edge_in;
    
	Node(int id);
	~Node();

	int getNodeId();
	double getCurrBanlance();


	void print(); 
};

#endif