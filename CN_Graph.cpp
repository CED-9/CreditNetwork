
#include "CN_Graph.h"
#include "CN_Constants.h"
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

extern CredNetConstants credNetConstants;

/////////////////////////////////////////////////////////////////////////
/* Graph basics */
/////////////////////////////////////////////////////////////////////////
Graph::Graph(){
	this->atomicGlobalId = 0;
}

Graph::Graph(int nodeNumT){
	this->nodeNum = nodeNumT;
	this->atomicGlobalId = 0;
	for (int i = 0; i < nodeNum; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}
}

static void deepCopyHelper(Graph* newGraph, Graph& oldGraph){

	newGraph->nodeNum = oldGraph.nodeNum;
	newGraph->atomicGlobalId = oldGraph.atomicGlobalId;
	for (auto& it : oldGraph.nodes){
		newGraph->nodes[it.first] = new Node(it.first);
		newGraph->nodes[it.first]->transactionNum = it.second->transactionNum;
		newGraph->nodes[it.first]->totalIR = it.second->totalIR;
		newGraph->nodes[it.first]->routePreference = it.second->routePreference;
	}
	for (auto& oldNodePair : oldGraph.nodes){
		for (auto& oldEdgePair : oldNodePair.second->edge_in){
			int fromId = oldEdgePair.first;
			int toId = oldNodePair.first;
			Node* newFromNode = newGraph->nodes[fromId];
			Node* newToNode = newGraph->nodes[toId];

			Edge* e = new Edge(*(oldEdgePair.second),
				newFromNode, newToNode, newGraph->atomicEdges);
			newToNode->edge_in[fromId] = e;
			newFromNode->edge_out[toId] = e;
			newGraph->edges.push_back(e);
		}
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
	for (auto& it : edges){
		delete it;
	}
}


void Graph::updateNodeDegrees(){
	for (auto& it : this->nodes){
		it.second->updateDegree();
	}
}

void Graph::addMultiEdge(Node* nodeFrom, Node* nodeTo, 
	double credit_ir, double debt_ir, int currDebt, int cap){

	// use diff ir to create ious
	Edge* e;
	if (nodeTo->edge_in.find(nodeFrom->getNodeId()) == nodeTo->edge_in.end()){
		e = new Edge(nodeFrom, nodeTo);
		nodeTo->edge_in[nodeFrom->getNodeId()] = e;
		nodeFrom->edge_out[nodeTo->getNodeId()] = e;
		this->edges.push_back(e);
	} else {
		e = nodeTo->edge_in[nodeFrom->getNodeId()];
	}

	SingleCreditEdge* temp = e->addSingleCreditEdge(credit_ir, cap, this->atomicGlobalId, this->atomicEdges);
	if (currDebt != 0){
		e->routeAtomicEdge(temp->credit_remain, currDebt, debt_ir, atomicGlobalId, this->atomicEdges, 0);
	}

}

void Graph::print(){
	for (auto& it : nodes){
		it.second->print();
	}
}

void Graph::printAtomicEdges(){
	cout << "--atomic edges of graph: ---------------------------------------" << endl;
	for (auto &it : this->atomicEdges){
		it.second->print();
	}
	cout << endl;

}

void Graph::printAvgAtomicIouEdges(){
	int cnt = 0;
	double totalIrCap = 0;
	double totalCap = 0;

	for (auto &it : this->atomicEdges){
		if (!it.second->isDebt || it.second->capacity == 0){
			continue;
		}
		totalIrCap += it.second->capacity * it.second->interest_rate;
		totalCap += it.second->capacity;
		cnt ++;
	}
	cout << "   " << totalIrCap / totalCap << endl;
}

void Graph::printAtomicIouEdges(ofstream & fout){
	fout << "--All IOU Edges ----------------------------------------------" << endl;
	fout << "nodeFrom \t nodeTo \t amount \t ir" << endl;
	for (auto &it : this->atomicEdges){
		if (!it.second->isDebt || it.second->capacity == 0){
			continue;
		}
		fout << it.second->nodeFrom->nodeId << " \t "
			<< it.second->nodeTo->nodeId << " \t "
			<< it.second->capacity << " \t "
			<< it.second->interest_rate << endl;
	}
	fout << endl;
}




/////////////////////////////////////////////////////////////////////////
/* Generate Initial Network */
/////////////////////////////////////////////////////////////////////////
void Graph::genTest0Graph(double threshold, int numIR, int cap){

	for (int i = 0; i < nodeNum; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}

	// default_random_engine generator;
	// uniform_real_distribution<double> distribution(0.0, 1.0);
	for (int i = 0; i < nodeNum; i++){
		for (int j = i+1; j < nodeNum; j++){

			double num = credNetConstants.uniformDoubleDistribution(
				credNetConstants.gloabalGenerator);

			double ir = (credNetConstants.uniformIntDistribution(
				credNetConstants.gloabalGenerator) % numIR + 1);
			
			if (num < threshold){
				if (rand()%2 == 1) {
					// this->addUnitEdge(nodes.find(i)->second, nodes.find(j)->second, ir, rand()%2);
					this->addMultiEdge(nodes.find(i)->second, nodes.find(j)->second, ir, 0.0, 0, cap);
				} else {
					// this->addUnitEdge(nodes.find(j)->second, nodes.find(i)->second, ir, rand()%2);
					this->addMultiEdge(nodes.find(j)->second, nodes.find(i)->second, ir, 0.0, 0, cap);
				}
			}

		}
	}


}

void Graph::generateTestGraph2(){

	nodeNum = 8;
	for (int i = 0; i < 8; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}
	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.2, 0.4, 2, 2);

	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.3, 0.3, 3, 3);

	this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.3, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(2)->second, nodes.find(4)->second, 0.2, 0.2, 4, 4);

	this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.2, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.4, 0.0, 0, 1);

	this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.2, 0.0, 0, 3);

	this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.4, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(6)->second, nodes.find(1)->second, 0.2, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(7)->second, nodes.find(6)->second, 0.3, 0.0, 0, 1);

	this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.2, 0.0, 0, 2);

}


void Graph::generateTestGraph3(){

	nodeNum = 4;
	for (int i = 0; i < nodeNum; ++i){
		nodes[i] = new Node(i);
	}

	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.02, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(0)->second, nodes.find(2)->second, 0.03, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(1)->second, nodes.find(0)->second, 0.02, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(1)->second, nodes.find(2)->second, 0.01, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(1)->second, nodes.find(3)->second, 0.01, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(2)->second, nodes.find(0)->second, 0.03, 0.0, 0, 2);

	this->addMultiEdge(nodes.find(3)->second, nodes.find(2)->second, 0.02, 0.0, 0, 2);

}



void Graph::setRoutePreference(vector<string> &v){
     
	for (int i = 0; i < nodeNum; i++) {
		nodes[i]->routePreference = v[i];
	}

	return;
}



//////////////////////////////////////////////////////
// route on atomic edge
//////////////////////////////////////////////////////
void helpRouteOnAtomicEdge(int current, double interest_rate, AtomicEdge* a, Graph* g, int transSeqNum){
	a->originEdge->routeAtomicEdge(a, current, interest_rate, g->atomicGlobalId, g->atomicEdges, transSeqNum);
}
