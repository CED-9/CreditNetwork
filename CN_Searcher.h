#ifndef CN_Searcher
#define CN_Searcher

#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_WidgetGraph.h"

#include <list>

class Searcher{
public:
	static bool bfsWidget(WidgetGraph* widgetGraph, 
		Node* src, Node* dest, list<WidgetNode*>& path);

};

#endif