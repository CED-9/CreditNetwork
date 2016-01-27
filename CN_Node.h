#ifndef CN_Node
#define CN_Node

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// forward declaration
class Graph;
class Edge;
class AtomicEdge;
class WidgetNode;

class Node{

public:
	int nodeId;
	int transactionNum;
	double totalIR;
	string routePreference;
	int degree;

	// edges
	unordered_map<int, Edge*> edge_out;
	unordered_map<int, Edge*> edge_in;

	// atomic edges
	unordered_map<int, AtomicEdge*> atomicEdge_in;
	unordered_map<int, AtomicEdge*> atomicEdge_out;
    
	// widget nodes
	unordered_map<int, WidgetNode*> credit_out_widget_nodes;
	unordered_map<int, WidgetNode*> credit_in_widget_nodes;
	unordered_map<int, WidgetNode*> debt_in_widget_nodes;
	unordered_map<int, WidgetNode*> debt_out_widget_nodes;

	Node(int id);

	~Node();

	int getNodeId();
	double getCurrBanlance();
	void updateDegree();


	void print(); 
};

#endif