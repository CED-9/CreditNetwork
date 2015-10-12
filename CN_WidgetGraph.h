#ifndef CN_WidgetGraph
#define CN_WidgetGraph

#include "CN_Node.h"
#include "CN_Graph.h"
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum WidgetNodeType{
	CREDIT_IN_NODE, // Out Widget Edge
	DEBT_OUT_NODE, // Out Widget Edge
	CREDIT_OUT_NODE, // In Widget Edge
	DEBT_IN_NODE, // In Widget Edge
	SUPER_SOURCE,
	SUPER_DEST,
};

class WidgetNode;

struct WidgetEdge{
	WidgetNode* nodeFrom;
	WidgetNode* nodeTo;
	double cap;
	double curr;
	double interest_rate;
	double interest_diff;
	WidgetEdge(double ir, double ir_diff, int capT,  WidgetNode* nodeFromT, WidgetNode* nodeToT)
		: curr(0), cap(capT), interest_rate(ir), nodeTo(nodeToT), nodeFrom(nodeFromT), interest_diff(ir_diff) {}
};

static std::string helper(WidgetNodeType type){
	switch(type){
		case CREDIT_OUT_NODE:
			return "CREDIT_OUT_NODE";
		case CREDIT_IN_NODE:
			return "CREDIT_IN_NODE";
		case DEBT_OUT_NODE:
			return "DEBT_OUT_NODE";
		case DEBT_IN_NODE:
			return "DEBT_IN_NODE";
	}
}

class WidgetNode{
public:
	int globalNodeId;
	WidgetNodeType type;
	Node* originNode;
	int targetNodeId;

	unordered_map<int, WidgetEdge*> edge_out;
	unordered_map<int, WidgetEdge*> edge_in;

	WidgetNode(WidgetNodeType t, Node* o, int id, int targetNodeIdT)
		: type(t), originNode(o), globalNodeId(id), targetNodeId(targetNodeIdT) {}

	~WidgetNode(){
		for (auto it : edge_out){
			delete it.second;
		}
	}

	int getGlobalId(){
		return this->globalNodeId;
	}

	void print(){
		cout << "Global ID: " << this->globalNodeId << endl;
		cout << "origin node: " << originNode->getNodeId() << " " 
			<< helper(type) << ", target node: " << targetNodeId << endl;

		for (auto it : edge_in){
			cout << "From widget node " << it.second->nodeFrom->globalNodeId 
				<< " which belongs to " <<  it.second->nodeFrom->originNode->getNodeId() << endl
				<< "interest rate: " << it.second->interest_rate 
				<< " capacity: " << it.second->cap 
				<< " current: " << it.second->curr << endl;
		}

		for (auto it : edge_out){
			cout << "To widget node " << it.second->nodeTo->globalNodeId 
				<< " which belongs to " <<  it.second->nodeTo->originNode->getNodeId() << endl
				<< "interest rate: " << it.second->interest_rate 
				<< " capacity: " << it.second->cap 
				<< " current: " << it.second->curr << endl;
		}

		cout << endl;
	}
};

/**
 * all credit edges will be inversed, 
 * 
 */
class WidgetGraph{
private:
	Graph* originGraph;
public:
	Node* src;
	Node* dest;
	double payment;
	vector<WidgetNode*> widgetNodes;


	void addEdge(WidgetNode* node1, WidgetNode* node2, int capacity, double ir, double ir_diff);
	void constructWidget(Graph* graphT);
	void copyBack();

	WidgetGraph();
	~WidgetGraph();
	
	void print(){
		for (int i = 0; i < widgetNodes.size(); ++i){
			widgetNodes[i]->print();
		}
	}
	void setupSrcAndDest(Node*, Node*, double);
	int lpSolver(int opMode);
};




#endif