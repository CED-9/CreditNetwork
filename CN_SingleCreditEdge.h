#ifndef CN_SingleCreditEdge
#define CN_SingleCreditEdge

#include "CN_AtomicEdge.h"

// Single credit line, only one contract ir
class SingleCreditEdge{
public:

	int indexOfEdge;
	int contract_credit;
	double contract_interest_rate;

	AtomicEdge* credit_remain;  // credit line remaining
	unordered_map<double, AtomicEdge*> debt_current;  // ir->iou

	SingleCreditEdge(int ind, int c_max, double ir, 
		int& globalId, Edge* e, 
		Node* nodeFromT, Node* nodeToT)
	: contract_credit(c_max), contract_interest_rate(ir), indexOfEdge(ind){

		credit_remain = new AtomicEdge(false, globalId, 
			c_max, ir, e, this, nodeFromT, nodeToT);

	}

	~SingleCreditEdge(){
		delete credit_remain;
		for (auto it : debt_current){
			delete it.second;
		}
	}

};

#endif