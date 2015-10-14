
#include "CN_Node.h"
#include "CN_Graph.h"

#include "CN_WidgetGraph.h"
#include "CN_Searcher.h"

#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){

	Graph graph;
	graph.generateTestGraph();
	graph.print();

	// WidgetGraph widget;
	// widget.setupSrcAndDest(graph.nodes.find(0)->second, graph.nodes.find(5)->second, 1.0);
	// widget.constructWidget(&graph);
	// widget.print();

	vector<Node*> path;
	bool result = Searcher::bfsIRConstraint(0.5, &graph, graph.nodes.find(0)->second,
		graph.nodes.find(5)->second, path);

	cout << "result: " << result << endl;

	for (int i = 0; i < path.size(); ++i){
		cout << path[i]->getNodeId();
	}

	return 0;
}