
#include "CN_Graph.h"
#include <iostream>

using namespace std;


/////////////////////////////////////////////////////////////////////////
/* Graph basics */
/////////////////////////////////////////////////////////////////////////
Graph::Graph(){}

Graph::Graph(int nodeNumT){
	this->nodeNum = nodeNumT;
}

static void deepCopyHelper(Graph* newGraph, Graph& oldGraph){
	for (auto& it : oldGraph.nodes){
		std::pair<int, Node*> tempPair;
		tempPair.first = it.first;
		tempPair.second = it.second;
		newGraph->nodes.insert(tempPair);
	}
}

Graph::Graph(Graph &graphT){
	deepCopyHelper(this, graphT);
}

Graph& Graph::operator=(Graph &graphT){
	deepCopyHelper(this, graphT);
	return *this;
}

Graph::~Graph(){
	for (auto& it : nodes){
		delete it.second;
	}
}


void Graph::addUnitEdge(Node* nodeFrom, Node* nodeTo, double ir, int currDebt){

	std::pair<int, Edge*> edge;

	edge.first = nodeFrom->getNodeId();
	edge.second = new Edge(nodeFrom->getNodeId(), nodeTo->getNodeId(), ir);
	edge.second->setCurrent(currDebt);
	nodeTo->edge_in.insert(edge);

	edge.first = nodeTo->getNodeId();
	nodeFrom->edge_out.insert(edge);

}

void Graph::print(){
	for (auto& it : nodes){
		it.second->print();
	}
}

/////////////////////////////////////////////////////////////////////////
/* Generate Initial Network */
/////////////////////////////////////////////////////////////////////////
void Graph::generateTestGraph(){

	for (int i = 0; i < 6; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}
	
	this->addUnitEdge(nodes.find(1)->second, nodes.find(0)->second, 0.3, 0);
	this->addUnitEdge(nodes.find(1)->second, nodes.find(2)->second, 0.2, 0);
	this->addUnitEdge(nodes.find(3)->second, nodes.find(1)->second, 0.2, 0);
	this->addUnitEdge(nodes.find(4)->second, nodes.find(1)->second, 0.4, 0);
	this->addUnitEdge(nodes.find(2)->second, nodes.find(3)->second, 0.1, 0);
	this->addUnitEdge(nodes.find(3)->second, nodes.find(4)->second, 0.1, 0);
	this->addUnitEdge(nodes.find(5)->second, nodes.find(4)->second, 0.3, 0);
	this->addUnitEdge(nodes.find(5)->second, nodes.find(3)->second, 0.4, 0);
	this->addUnitEdge(nodes.find(5)->second, nodes.find(2)->second, 0.2, 0);
}
