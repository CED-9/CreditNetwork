
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_Searcher.h"
#include "CN_Executer.h"
#include "CN_CreditNet.h"

#include "CN_WidgetGraph.h"

#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

int main(int argc, char* argv[]){

	Graph graph;
	graph.generateTestGraph2();
	graph.print();

	WidgetGraph w;
	w.constructWidget(&graph);
	w.print();

	return 0;
}