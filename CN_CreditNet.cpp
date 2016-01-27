// CN_CreditNet.C 
#define NON_DEBUG_MODE

#include "CN_CreditNet.h"
#include <string>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

CreditNet::CreditNet(int finNumT) : Graph(finNumT){}

CreditNet::CreditNet() : Graph(){}

void CreditNet::printPayoff(){
	for (auto it : nodes) {
		cout << it.second->getNodeId() << ": "
			<< "Transactions " << it.second->transactionNum << ", Current Banlance "
			<< it.second->getCurrBanlance() << endl;
	}
}

// for liquid stuff
int CreditNet::genInterBankTrans(int request, string mode){

	Node* f1 = NULL;
	Node* f2 = NULL;
	nodeNum = nodes.size();
	int fid1 = rand()%nodeNum;
	f1 = nodes.find(fid1)->second;
	int fid2 = rand()%nodeNum;
	while (fid1 == fid2){
		fid2 = rand()%nodeNum;
	}
	f2 = nodes.find(fid2)->second;

	this->updateNodeDegrees();

	// this->print();
	//cout << "fid1: " << fid1 << " fid2: " << fid2 << endl;
	CplexConverter converter;
	converter.constructCplex(this, this->nodes[fid1], this->nodes[fid2], request);
	// converter.printInput();
	LpSolver lpSolver;

	if (lpSolver.solveLpProblem(converter, mode)){
		// converter.printResult();
		converter.copyBack();
		return 0;
	}
	return 1;
	
}