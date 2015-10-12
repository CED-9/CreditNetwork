#ifndef CN_Node
#define CN_Node

#include "CN_Edge.h"
#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;

// forward declaration
class Graph;
class WidgetNode;

class Node{

public:
	int nodeId;

    // edges
	unordered_map<int, OutEdge*> edge_out;
	unordered_map<int, InEdge*> edge_in;

	// widget nodes
	unordered_map<int, WidgetNode*> credit_out_widget_nodes;
	unordered_map<int, WidgetNode*> credit_in_widget_nodes;
	unordered_map<int, WidgetNode*> debt_in_widget_nodes;
	unordered_map<int, WidgetNode*> debt_out_widget_nodes;
    
	Node(int id);
	~Node();

	void setNodeId(int id){ nodeId = id; }
	int getNodeId(){ return nodeId; }

	void print(); 
};

#endif