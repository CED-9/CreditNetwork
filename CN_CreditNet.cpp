// CN_CreditNet.C 
#define NON_DEBUG_MODE

#include "CN_Constants.h"
#include "CN_CreditNet.h"
#include <string>
#include <queue>
#include <vector>
#include <iostream>
using namespace std;

extern CredNetConstants credNetConstants;

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
int CreditNet::genInterBankTrans(int request, string mode, int transSeqNum){

	Node* f1 = NULL;
	Node* f2 = NULL;
	nodeNum = nodes.size();

	int fid1 = credNetConstants.uniformIntDistribution(
		credNetConstants.gloabalGenerator)%nodeNum;
	int fid2 = credNetConstants.uniformIntDistribution(
		credNetConstants.gloabalGenerator)%nodeNum;
	while (fid1 == fid2){
		fid2 = credNetConstants.uniformIntDistribution(
			credNetConstants.gloabalGenerator)%nodeNum;
	}
	
	// int fid1 = rand()%nodeNum;
	// int fid2 = rand()%nodeNum;
	// while (fid1 == fid2){
	// 	fid2 = rand()%nodeNum;
	// }

	this->updateNodeDegrees();
	if (mode == "SRC_DECIDE"){
		mode = this->nodes[fid1]->routePreference;
	}

	// this->print();

	// fid1 = 2; fid2 = 0;
	// cout << "from " << fid1 << ", to " << fid2 << " " << request << " " << mode;

	CplexConverter converter;
	converter.constructCplex(this, this->nodes[fid1], this->nodes[fid2], request, transSeqNum);

	// converter.printInput();
	
	LpSolver lpSolver;

	this->nodes[fid1]->srcNum++;
	this->nodes[fid2]->destNum++;
	
	if (lpSolver.solveLpProblem(converter, mode)){
		// converter.printResult();
		converter.copyBack();
		this->nodes[fid1]->transactionNum++;
		this->nodes[fid1]->successSrc++;
		this->nodes[fid2]->successDest++;
		// cout << " success " << endl;
		return 0;
	}
	// cout << " fail " << endl;
	return 1;
	
}