#include "CN_Edge.h"
#include "CN_Graph.h"
#include "CN_WidgetGraph.h"
#include "CN_CplexSolver.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <random>

using namespace std;



int main(int argc, char* argv[]){

	// Node from(0), to(1);
	// int globalId(0);
	// Edge edge(&from, &to);
	// unordered_map<int, AtomicEdge*> atomicMap;

	// edge.addSingleCreditEdge(1.0, 100, globalId, atomicMap);
	// edge.addSingleCreditEdge(2.0, 200, globalId, atomicMap);
	// edge.addSingleCreditEdge(3.0, 300, globalId, atomicMap);
	// edge.print();

	// cout << "after -------------------------------------------" << endl;
	// AtomicEdge* a = edge.singleCreditEdges[0]->credit_remain;
	// edge.routeAtomicEdge(a, 10, 3.0, globalId, atomicMap);
	// edge.routeAtomicEdge(a, 10, 4.0, globalId, atomicMap);
	// AtomicEdge* b = edge.singleCreditEdges[0]->debt_current.find(4.0)->second;
	// edge.routeAtomicEdge(b, 5, 0.1, globalId, atomicMap);
	// edge.print();

	// cout << "atomic edges ---------------------------------" << endl;
	// for (auto it : atomicMap){
	// 	it.second->print();
	// }
	

	Graph g;
	g.generateTestGraph2();
	cout << "--------------------------------------" << endl;
	g.print();

	WidgetGraph widget;
	widget.constructWidget(&g);
	widget.setupSrcAndDest(g.nodes[0], g.nodes[5], 5);

	cout << "widget graph --------------------------------" << endl;
	widget.print();

	CplexConverter converter;
	converter.constructCplex(&widget);
	cout << "--------------------------------------" << endl;
	converter.printInput();

	CplexSolver solver;
	cout << "--------------------------------------" << endl;
	if (solver.solve(converter) == 0){
		converter.copyBack();
	}
	converter.printResult();


	cout << "copy back --------------------------------" << endl;
	widget.copyBack();
	g.print();




	return 0;
}