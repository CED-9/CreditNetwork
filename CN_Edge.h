#ifndef CN_Edge
#define CN_Edge

#include "CN_Node.h"

class OutEdge{
	Node* nodeTo;
	double c_out_max;
	double d_in_current;
	double interest_rate;
};

class InEdge{
	Node* nodeFrom;
	double c_in_max;
	double d_out_current;
	double interest_rate;
};

#endif