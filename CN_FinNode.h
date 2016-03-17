#ifndef CN_FinNode
#define CN_FinNode

#include "CN_Edge.h"
#include "CN_Node.h"
#include "CN_Graph.h"

class FinNode : public Node{

public:
	int customerWeight;
	int produverWeight;

};

#endif