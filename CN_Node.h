#ifndef CN_Node
#define CN_Node

#include "CN_Constants.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;


class Node{

public:
	int nodeId;
	int transactionNum;

	int srcNum;
	int destNum;
	int successSrc;
	int successDest;
	vector<int> transSeq;

	double totalIR;
	string routePreference;
	int degree;

	// edges
	unordered_map<int, Edge*> edge_out;
	unordered_map<int, Edge*> edge_in;

	// atomic edges
	unordered_map<int, AtomicEdge*> atomicEdge_in;
	unordered_map<int, AtomicEdge*> atomicEdge_out;


	Node(int id);
	~Node();
	int getNodeId();
	double getCurrBanlance();
	void updateDegree();
	void printTransSeq();
	void addModification(int transSeqNum);
	void print(); 
};

#endif