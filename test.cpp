
#include "CN_Node.h"
#include "CN_Graph.h"

#include "CN_WidgetGraph.h"
#include "CN_Searcher.h"

#include <unordered_map>
#include <list>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){

	Graph graph;
	graph.generateTestGraph();
	graph.print();

	WidgetGraph widget;
	widget.setupSrcAndDest(graph.nodes.find(0)->second, graph.nodes.find(5)->second, 1.0);
	widget.constructWidget(&graph);
	widget.print();

	list<WidgetNode*> path;
	bool result = Searcher::bfsWidget(&widget, widget.widgetNodes[17]->originNode, 
		widget.widgetNodes[0]->originNode, path);

	cout << "result: " << result << endl;

	while (!path.empty()){
		cout << path.front()->getGlobalId() << endl;
		path.pop_front();
	}

	return 0;
}