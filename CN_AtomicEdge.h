#ifndef CN_AtomicEdge
#define CN_AtomicEdge

#include "CN_Node.h"

// atomic edge
// debt or credit with capacity
class AtomicEdge{
public:
	// original Edge Info
	Edge* originEdge;
	singleCreditEdge* originSingleCreditEdge;

	bool isDebt;
	int atomicEdgeId;
	int capacity;
	double interest_rate;

	Node* nodeFrom;
	Node* nodeTo;

	AtomicEdge(bool d, int id, int capT, double ir, 
		Edge* e, singleCreditEdge* s, Node* from, Node* to)
	: isDebt(d), atomicEdgeId(id)
		, capacity(capT), interest_rate(ir)
		, originEdge(e), originSingleCreditEdge(s)
		, nodeFrom(from), nodeTo(to)
	{}



	void print() {
		cout << "    Atomic Edge, id: " << atomicEdgeId << " " 
		<< "capacity: " << capacity 
		<< " ir: " << interest_rate 
		<< " isDebt: " << isDebt 
		<< " single index " << singleCreditIndex 
		<< " from " << nodeFrom->nodeId 
		<< " to " << nodeTo->nodeId
		<< endl;
	}
};

#endif