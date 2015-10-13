#ifndef CN_Searcher
#define CN_Searcher

#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_WidgetGraph.h"

#include <list>
#include <vector>

class Searcher{
public:

	static bool dfs(Graph* graph, 
		Node* src, Node* dest, vector<Node*>& path);

	static bool bfsWidget(WidgetGraph* widgetGraph, 
		Node* src, Node* dest, list<WidgetNode*>& path);

};

#endif