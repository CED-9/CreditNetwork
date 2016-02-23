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

void simulation();

int main(int argc, char* argv[]){

	credNetConstants.addIr(0.1);
	credNetConstants.addIr(0.2);
	credNetConstants.addIr(0.3);

	/* test graph */
	CreditNet g;
	g.generateTestGraph3();
	g.updateNodeDegrees();

	vector<string> v;
	for (int i = 0; i < g.nodes.size(); ++i){
		v.push_back("MIN_SRC_COST");
	}
	g.setRoutePreference(v);

	for (int i = 0; i < 1; ++i){
		simulation(g);
	}
	
	g.print();


	return 0;
}

void simulation(CreditNet g){
	int fid1 = credNetConstants.uniformIntDistribution(
		credNetConstants.gloabalGenerator)%nodeNum;
	int fid2 = credNetConstants.uniformIntDistribution(
		credNetConstants.gloabalGenerator)%nodeNum;
	while (fid1 == fid2){
		fid2 = credNetConstants.uniformIntDistribution(
			credNetConstants.gloabalGenerator)%nodeNum;
	}
	CplexConverter converter;
	converter.constructCplex(&g, g.nodes[fid1], g.nodes[fid2], 10, 0);
	LpSolver lpSolver;
	cout << "routing from node " << fid1 << " to node " << fid2 << " succ " lpSolver.solveLpProblem(converter, mode) << endl;
}