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
#include <string>

using namespace std;

extern CredNetConstants credNetConstants;

void simulation(string strategy_1, string strategy_99);
void simulationNoStatistics(string strategy_1, string strategy_99);


int main(int argc, char* argv[]){

	if (argc < 2){
		cerr << "enter more" << endl;
		return -1;
	}

	string strategy_1 = argv[1];
	string strategy_99 = argv[2];

	credNetConstants.addIr(1);
	credNetConstants.addIr(2);
	credNetConstants.addIr(3);
	credNetConstants.addIr(4);

	cout << "NodeId, \tMecha, \tDegree, \tsuccSrc, \tsuccDest, \tiouIr, \tnodeEngage ";

	for (int i = 0; i < 15; ++i){
		simulationNoStatistics(strategy_1, strategy_99);
	}

	return 0;
}

void simulationNoStatistics(string strategy_1, string strategy_99){

	/* test graph */
	CreditNet g(100);
	int req = 10;
	int cap = 10;
	int numIR = 4;
	double threshold = 0.05;

	g.genTest0Graph(threshold, numIR, cap);
	g.updateNodeDegrees();

	vector<string> v;
	for (int i = 0; i < 99; ++i){
		v.push_back(strategy_99);
	}
	v.push_back(strategy_1);
	g.setRoutePreference(v);


	for (int i = 0; i < 10000; ++i){
		g.genInterBankTrans(10, "SRC_DECIDE", i);
	}

	for (int i = 0; i < g.nodes.size(); ++i){

		cout << g.nodes[i]->nodeId 
			<< ", \t" << g.nodes[i]->routePreference
			<< ", \t" << g.nodes[i]->degree 
			<< ", \t" << g.nodes[i]->successSrc
			<< ", \t" << g.nodes[i]->successDest 
			<< ", \t" << g.nodes[i]->getCurrBanlance() 
			<< ", \t" << g.nodes[i]->transSeq.size() << endl;
	}

}





void simulation(string strategy_1, string strategy_99){

	/* test graph */
	CreditNet g(100);
	int req = 10;
	int cap = 10;
	int numIR = 4;
	double threshold = 0.05;

	g.genTest0Graph(threshold, numIR, cap);
	g.updateNodeDegrees();

	vector<string> v;
	for (int i = 0; i < 99; ++i){
		v.push_back(strategy_99);
	}
	v.push_back(strategy_1);
	g.setRoutePreference(v);


	for (int i = 0; i < 10000; ++i){
		g.genInterBankTrans(10, "SRC_DECIDE", i);
	}

	double avgTransSucc = 0;
	double avgIouIr = 0;
	double avgDegree = 0;
	double avgNodeEngage = 0;
	double avgEdgeEngage = 0;
	double totalEdgeNum = 0;

	double avgTargetDegreeNodeTrans = 0;
	double avgTargetDegreeNodeIouIr = 0;
	double avgTargetDegreeNodeEngage = 0;
	double totalTargetDegreeNodeNum = 0;
	int targetNodeDegree = g.nodes[99]->degree;

	for (auto& it : g.nodes){
		// it.second->print();
		// cout << "node id: " << it.first << it.second->degree << " \t"
		// 	<< "src num " << it.second->srcNum
		// 	<< " \tdest num " << it.second->destNum
		// 	<< " \tsucc src num " << it.second->successSrc 
		// 	<< " \tsucc dest num " << it.second->successDest
		// 	<< " \tiou ir " << it.second->getCurrBanlance()
		// 	<< endl;

		// cout << "node engage: ";
		// it.second->printTransSeq();

		// cout << "edge engage: \n";
		// for (auto& edge : it.second->edge_in){

		// 	cout << "Edge from " << edge.second->nodeFrom->nodeId << " to " << edge.second->nodeTo->nodeId << endl;

		// 	avgEdgeEngage += edge.second->edgeUsage.size();
		// 	for (int j = 0; j < edge.second->edgeUsage.size(); ++j){
		// 		cout << edge.second->edgeUsage[j] << " ";
		// 	}
		// 	cout << endl;
		// }

		totalEdgeNum += it.second->edge_in.size();
		avgDegree += it.second->degree;
		avgNodeEngage += it.second->transSeq.size();
		avgTransSucc += it.second->successSrc;
		avgIouIr += it.second->getCurrBanlance();

		if (it.second->degree == targetNodeDegree){
			avgTargetDegreeNodeTrans += it.second->successSrc;
			avgTargetDegreeNodeIouIr += it.second->getCurrBanlance();
			avgTargetDegreeNodeEngage += it.second->transSeq.size();
			totalTargetDegreeNodeNum += 1;
		}
	}

	avgTransSucc /= 100;
	avgIouIr /= 100;
	avgDegree /= 100;
	avgNodeEngage /= 100;
	avgEdgeEngage /= totalEdgeNum;

	avgTargetDegreeNodeTrans /= totalTargetDegreeNodeNum;
	avgTargetDegreeNodeIouIr /= totalTargetDegreeNodeNum;
	avgTargetDegreeNodeEngage /= totalTargetDegreeNodeNum;


	cout << endl;
	cout << "avg success " << avgTransSucc << endl;
	cout << "avg iou ir " << avgIouIr << endl;
	cout << "avg degree " << avgDegree << endl;
	cout << "avg node engage " << avgNodeEngage << endl;
	// cout << "avg edge engage " << avgEdgeEngage << endl;

	cout << "degree " << targetNodeDegree << " " << "avg success " << avgTargetDegreeNodeTrans << endl;
	cout << "degree " << targetNodeDegree << " " << "avg iou ir " << avgTargetDegreeNodeIouIr << endl;
	cout << "degree " << targetNodeDegree << " " << "avg node engage " << avgTargetDegreeNodeEngage << endl;

	cout << "node 99 degree " << g.nodes[99]->degree << endl;
	cout << "node 99 success " << g.nodes[99]->successSrc << endl;
	cout << "node 99 iou ir " << g.nodes[99]->getCurrBanlance() << endl;
	cout << "node 99 node engage " << g.nodes[99]->transSeq.size() << endl;
}