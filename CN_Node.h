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
class WidgetNode;

class Node{

public:
	int nodeId;
	int transactionNum;
	double totalIR;
	string routePreference;

	// edges
	unordered_map<int, Edge*> edge_out;
	unordered_map<int, Edge*> edge_in;
    
	// widget nodes
	unordered_map<int, WidgetNode*> credit_out_widget_nodes;
	unordered_map<int, WidgetNode*> credit_in_widget_nodes;
	unordered_map<int, WidgetNode*> debt_in_widget_nodes;
	unordered_map<int, WidgetNode*> debt_out_widget_nodes;

	Node(int id);
	Node(const Node& n);
	~Node();

	int getNodeId();
	double getCurrBanlance();


	void print(); 
};

#endif