// Node.C

#include "CN_Node.h"
#include <queue>
#include <vector>
#include <fstream>


Node::Node(int id){
	this->nodeId = id;
	this->transactionNum = 0;
	this->totalIR = 0;
	this->routePreference = "";
	srcNum = 0;
	destNum = 0;
	successSrc = 0;
	successDest = 0;
}

Node::~Node(){}



void Node::addModification(int transSeqNum){
	this->transSeq.push_back(transSeqNum);
}

void Node::printTransSeq(){
	for (int i = 0; i < transSeq.size(); ++i){
		cout << transSeq[i] << " ";
	}
	cout << endl;
}