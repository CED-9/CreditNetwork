#include "CN_Constants.h"
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_Solver.h"
#include "CN_CreditNet.h"

#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <mutex>
#include <thread>
#include <cmath>

using namespace std;

extern CredNetConstants credNetConstants;

int main(){

	credNetConstants.addIr(1);
	credNetConstants.addIr(2);
	credNetConstants.addIr(3);
	credNetConstants.addIr(4);

	credNetConstants.print();

	/* test graph */
	CreditNet g(100);
	int req = 10;
	int cap = 10;
	int numIR = 4;
	double threshold = 0.05;

	g.genTest0Graph(threshold, numIR, cap);
	g.updateNodeDegrees();

	vector<string> v;
	v.push_back("MIN_SRC_COST");
	for (int i = 0; i < 99; ++i){
		v.push_back("MIN_CREDIT_COST");
	}
	g.setRoutePreference(v);


	for (int i = 0; i < 10000; ++i){
		g.genInterBankTrans(10, "SRC_DECIDE", i);
	}


	for (auto& it : g.nodes){
		cout << "node id: " << it.first
			<< "\tsrc num " << it.second->srcNum
			<< "\tdest num " << it.second->destNum
			<< "\tsucc src num " << it.second->successSrc 
			<< "\tsucc dest num " << it.second->successDest
			<< "\tiou ir " << it.second->getCurrBanlance()
			<< endl;
	}


	cout << "// Node Seq //////////////////////////////////////////////" << endl;
	for (auto& it : g.nodes){
		cout << "node id: " << it.first << "\t degree: " << it.second->degree;
		it.second->printTransSeq();
	}

	cout << "// Edge Seq //////////////////////////////////////////////" << endl;
	for (auto& it : g.nodes){
		cout << "node id: " << it.first << "\n";
		for (auto& edge : it.second->edge_in){

			cout << "Edge from " << edge.second->nodeFrom->nodeId << " to " << edge.second->nodeTo->nodeId << endl;

			for (int j = 0; j < edge.second->edgeUsage.size(); ++j){
				cout << edge.second->edgeUsage[j] << " ";
			}
			cout << endl;
		}
	}

	
	cout << "////////////////////////////////////////////////" << endl;

	g.print();

	return 0;
}