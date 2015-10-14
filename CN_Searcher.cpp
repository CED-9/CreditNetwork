#include "CN_Searcher.h"

#define NODE_NUM_MAX 1000

#include <set>
#include <queue>
#include <list>
#include <stack>
#include <unordered_map>
#include <iostream>

using namespace std;

bool Searcher::bfsIRConstraint(double ir, 
	Graph* graph, Node* src, Node* dest, vector<Node*>& path){

	path.clear();

	queue < pair<Node*, double> > tempQueue;
	unordered_map<int, double> tempVisited; // nodeId, max ir
	pair<Node*, double> frontPair;
	Node* prev[NODE_NUM_MAX];

	// initialization
	for (auto edge : src->edge_in){

		if (edge.second->get_c_in_remain() == 0 ||
			ir < edge.second->get_interest_rate()){
			continue;
		}

		pair<int, double> visitedPair;
		visitedPair.first = edge.second->getNodeFromId();
		visitedPair.second = ir;
		tempVisited.insert(visitedPair);

		pair<Node*, double> queuePair;
		queuePair.first = graph->nodes.find(edge.second->getNodeFromId())->second;
		queuePair.second = ir;
		tempQueue.push(queuePair);

	}
	for (auto edge : src->edge_out){

		if (edge.second->get_d_in_current() == 0 ||
			ir < edge.second->get_interest_rate()){
			continue;
		}

		pair<int, double> visitedPair;
		visitedPair.first = edge.second->getNodeToId();
		visitedPair.second = ir;
		tempVisited.insert(visitedPair);

		pair<Node*, double> queuePair;
		queuePair.first = graph->nodes.find(edge.second->getNodeToId())->second;
		queuePair.second = ir;
		tempQueue.push(queuePair);

	}
	

	for (int i = 0; i < NODE_NUM_MAX; ++i){
		prev[i] = NULL;
	}

	cout << "initialized " << tempQueue.size() << endl;

	while(tempQueue.size() != 0){

		frontPair = tempQueue.front();
		double currIR = frontPair.second;
		Node* front = frontPair.first;
		tempQueue.pop();

		cout << "front: " << front->getNodeId() << endl;

		if(dest == front){
			break;
		}

		// push to queue
		for(auto edge : front->edge_in){ 

			// cout << "front->edge " << edge.second->getNodeFromId() << endl;
			if (edge.second->get_c_in_remain() == 0){
				continue;
			}
			if(tempVisited.end() == tempVisited.find(edge.second->getNodeFromId())){
				pair<int, double> visitedPair;
				visitedPair.first = edge.second->getNodeFromId();
				visitedPair.second = currIR;
				tempVisited.insert(visitedPair);
			} else if (tempVisited.find(edge.second->getNodeFromId())->second >= ir){
				tempVisited.find(edge.second->getNodeFromId())->second = currIR;
			} else {
				continue;
			}

			prev[edge.second->getNodeFromId()] = front;

			pair<Node*, double> queuePair;
			queuePair.first = graph->nodes.find(edge.second->getNodeFromId())->second;
			queuePair.second = ir;
			tempQueue.push(queuePair);

		}

		for(auto edge : front->edge_out){ 
			if (edge.second->get_d_in_current() == 0){
				continue;
			}
			if(tempVisited.end() != tempVisited.find(edge.second->getNodeToId())){
				pair<int, double> visitedPair;
				visitedPair.first = edge.second->getNodeToId();
				visitedPair.second = currIR;
				tempVisited.insert(visitedPair);
			} else if (tempVisited.find(edge.second->getNodeToId())->second >= ir){
				tempVisited.find(edge.second->getNodeToId())->second = currIR;
			} else {
				continue;
			}

			prev[edge.second->getNodeToId()] = front;

			pair<Node*, double> queuePair;
			queuePair.first = graph->nodes.find(edge.second->getNodeToId())->second;
			queuePair.second = ir;
			tempQueue.push(queuePair);
		}

	}

	Node* result = frontPair.first;
	if(result != dest){
		path.clear(); 
		return false; 
	}

	while(result){
		path.push_back(result); 
		result = prev[result->getNodeId()];
	}

	return true;
}

