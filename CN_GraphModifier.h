#ifndef CN_GraphModifier
#define CN_GraphModifier

#include "CN_Constants.h"
#include "CN_Node.h"
#include "CN_AtomicEdge.h"
#include "CN_SingleCreditEdge.h"
#include "CN_Edge.h"
#include "CN_Graph.h"


class GraphModifier{
private:
	GraphModifier(){}
	
public:
	static double getNodeCurrBanlance(Node*);



	static void print(AtomicEdge* at);
	static void print(SingleCreditEdge* sc);
	static void print(Edge* e);
	static void print(Node* n);
	static void print(Graph* n);

};






#endif