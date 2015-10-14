#ifndef CN_Node
#define CN_Node

#include "CN_Edge.h"
#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;

// forward declaration
class Graph;
class Edge;

class Node{

public:
	int nodeId;

    // edges
	unordered_map<int, Edge*> edge_out;
	unordered_map<int, Edge*> edge_in;

	// widget nodes
	// unordered_map<int, WidgetNode*> credit_out_widget_nodes;
	// unordered_map<int, WidgetNode*> credit_in_widget_nodes;
	// unordered_map<int, WidgetNode*> debt_in_widget_nodes;
	// unordered_map<int, WidgetNode*> debt_out_widget_nodes;
    
	Node(int id);
	~Node();

	int getNodeId();

	void print(); 
};

#endif