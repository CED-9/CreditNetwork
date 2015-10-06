#ifndef CN_Node
#define CN_Node

#include <vector>
#include <iostream>

using namespace std;

// forward declaration
class Node;
class Graph;
#include "CN_Edge.h"


class Node{

public:
	int nodeId;

    // edges
	std::vector<OutEdge> edge_out;
	std::vector<InEdge> edge_in;
    
	Node(int id);

	void setNodeId(int id){ nodeId = id; }
	int getNodeId(){ return nodeId; }

	// // double change
	// Status setInEdge(
	// 	Node* nodeT, double c_in_maxT, 
	// 	double d_out_currentT, double interest_rateT, OpMode mode
	// 	); 
	// Status setOutEdge(
	// 	Node* nodeT, double c_out_maxT, 
	// 	double d_in_currentT, double interest_rateT, OpMode mode
	// 	); 

	void visualize();
	void print(); 
};

#endif