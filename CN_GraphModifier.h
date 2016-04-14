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

	// Edge Modifications
	static void setRoutePreference(vector<string> &v, Graph& g);
	static bool routeAtomicEdge(Graph* g, AtomicEdge* at, 
		int amt, double ir);
	static void addEdge(Graph* g, Node* nodeFrom, Node* nodeTo, 
		double credit_ir, double debt_ir, int currDebt, int cap);
	static void updateNodeDegrees(Graph* g);



	// Get Statistics
	static double getNodeCurrBanlance(Node*);
	static double getAvgAtomicIouEdges(Graph* g);
	

	// Print Out Status
	static void print(AtomicEdge* at);
	static void print(SingleCreditEdge* sc);
	static void print(Edge* e);
	static void print(Node* n);
	static void print(Graph* n);

};






#endif