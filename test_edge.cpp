#include "CN_Edge.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <random>

using namespace std;



int main(int argc, char* argv[]){

	Node from(0), to(1);
	int globalId;
	Edge edge(&from, &to);
	edge.addSingleCreditEdge(1.0, 100, globalId);
	edge.addSingleCreditEdge(2.0, 100, globalId);
	edge.addSingleCreditEdge(3.0, 100, globalId);
	edge.print();

	cout << "after -------------------------------------------" << endl;
	AtomicEdge* a = edge.singleCreditEdges[0]->credit_remain;
	edge.routeAtomicEdge(a, 10, 3.0, globalId);
	edge.routeAtomicEdge(a, 10, 4.0, globalId);
	AtomicEdge* b = edge.singleCreditEdges[0]->debt_current.find(4.0)->second;
	edge.routeAtomicEdge(b, 10, 0.1, globalId);
	edge.print();

	

	return 0;
}