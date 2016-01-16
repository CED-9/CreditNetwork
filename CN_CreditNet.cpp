// CN_CreditNet.C 
#define NON_DEBUG_MODE

#include "CN_CreditNet.h"
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
int CreditNet::genInterBankTrans(int request){

	Node* f1 = NULL;
	Node* f2 = NULL;
	
	int fid1 = rand()%nodeNum;
	f1 = nodes.find(fid1)->second;
	int fid2 = rand()%nodeNum;
	while (fid1 == fid2){
		fid2 = rand()%nodeNum;
	}
	f2 = nodes.find(fid2)->second;

	// this->print();
	cout << "fid1: " << fid1 << " fid2: " << fid2 << endl;
	CplexConverter converter;
	converter.constructCplex(this, this->nodes[fid1], this->nodes[fid2], request);
	// converter.printInput();
	LpSolver lpSolver;

	if (lpSolver.solveLpProblem(converter)){
		// converter.printResult();
		converter.copyBack();
		return 0;
	}
	return 1;

	
	// this->print();
	// cout << "fid1: " << fid1 << " fid2: " << fid2 << endl;
	// cout << f1->routePreference<<endl;
	// if (f1->routePreference == "BFS_SHORT") {
	// 	vector<Edge*> path;
	// 	bool result = Searcher::bfsIRConstraint(10.0, this, f1, f2, path);
	// 	if (result){
	// 		Executer::execute(path, f1, f2);
	// 		return 0;
	// 	}
	// 	return 1;
	// } else if (f1->routePreference == "BFS_LOW_IR") {
	// 	vector<Edge*> path;
	// 	int counter = 1;
	// 	double ir = counter/100.0;
	// 	while (!Searcher::bfsIRConstraint(ir, this, f1, f2, path)) {
	// 		counter += 1;
	// 		ir = counter/100.0;
	// 		if (counter == 10){
	// 			return 1;
	// 		}
	// 		// cout << "ir: " << ir << endl;
	// 	}
	// 	// cout << "-----------------------ir " << ir << endl;
	// 	// Searcher::bfsIRConstraint((counter+1)/100.0, this, f1, f2, path);
	// 	Executer::execute(path, f1, f2);
	// 	return 0;
	// }
	
	// error
	cerr << "fail routing! " << endl;
	return 0;
}