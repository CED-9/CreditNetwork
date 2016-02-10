#ifndef CN_Edge
#define CN_Edge

#include "CN_Node.h"
#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;

extern void helpRouteOnAtomicEdge(int current, double interest_rate, AtomicEdge* a, Graph* g, int transSeqNum);

// atomic edge, 0 flow, capacity
class AtomicEdge{
public:
	// original Edge Info
	Edge* originEdge;
	int singleCreditIndex;

	bool isDebt;
	int atomicEdgeId;
	int capacity;
	double interest_rate;
    int degree;

	Node* nodeFrom;
	Node* nodeTo;

	// widget stuff
	WidgetNode* fromWidget;
	WidgetNode* toWidget;

	AtomicEdge(const AtomicEdge& a, Edge* e, 
			int single, unordered_map<int, AtomicEdge*>& atomicMap, Node* nodeFromT, Node* nodeToT){
		this->originEdge = e;
		this->singleCreditIndex = single;
		this->isDebt = a.isDebt;
		this->atomicEdgeId = a.atomicEdgeId;
		this->capacity = a.capacity;
		this->interest_rate = a.interest_rate;
		atomicMap[this->atomicEdgeId] = this;
		nodeFrom = nodeFromT;
		nodeTo = nodeToT;
		if (this->isDebt){
			nodeFromT->atomicEdge_in[this->atomicEdgeId] = this;
			nodeToT->atomicEdge_out[this->atomicEdgeId] = this;
		} else {
			nodeFromT->atomicEdge_out[this->atomicEdgeId] = this;
			nodeToT->atomicEdge_in[this->atomicEdgeId] = this;
		}

		
	}

	AtomicEdge(bool d, int id, int capT, double ir, Edge* e, 
		int single, unordered_map<int, AtomicEdge*>& atomicMap, Node* nodeFromT, Node* nodeToT)
		: isDebt(d), atomicEdgeId(id), capacity(capT)
		, interest_rate(ir), originEdge(e), singleCreditIndex(single) {

			atomicMap[this->atomicEdgeId] = this;
			nodeFrom = nodeFromT;
			nodeTo = nodeToT;
			if (this->isDebt){
				nodeFromT->atomicEdge_in[this->atomicEdgeId] = this;
				nodeToT->atomicEdge_out[this->atomicEdgeId] = this;
			} else {
				nodeFromT->atomicEdge_out[this->atomicEdgeId] = this;
				nodeToT->atomicEdge_in[this->atomicEdgeId] = this;
			}

	}

	void route(int current, double interest_rate, Graph* g, int transSeqNum){
		if (current == 0){
			return;
		}
		helpRouteOnAtomicEdge(current, interest_rate, this, g, transSeqNum);
	}

	void print() {
		cout << "    Atomic Edge, id: " << atomicEdgeId << " " 
		<< "capacity: " << capacity << " ir: " << interest_rate 
		<< " isDebt: " << isDebt 
		<< " single index " << singleCreditIndex 
		<< " from " << nodeFrom->nodeId << " to " << nodeTo->nodeId
		<< endl;
	}
};


class SingleCreditEdge{
public:

	int credit_max;
	double credit_interest_rate;

	AtomicEdge* credit_remain;
	unordered_map<double, AtomicEdge*> debt_current;

	SingleCreditEdge(const SingleCreditEdge& s, Edge* e, 
		int single, unordered_map<int, AtomicEdge*>& atomicMap, Node* nodeFromT, Node* nodeToT) {
		
		this->credit_max = s.credit_max;
		this->credit_interest_rate = s.credit_interest_rate;
		this->credit_remain = new AtomicEdge(*(s.credit_remain), e, single, atomicMap, nodeFromT, nodeToT);
		for (auto it : s.debt_current){
			this->debt_current[it.first] = 
				new AtomicEdge(*(it.second), e, single, atomicMap, nodeFromT, nodeToT);
		}

	}

	SingleCreditEdge(int c_max, double ir, int& globalId, Edge* e, 
		int single, unordered_map<int, AtomicEdge*>& atomicMap, Node* nodeFromT, Node* nodeToT)
		: credit_max(c_max), credit_interest_rate(ir){

			credit_remain = new AtomicEdge(0, 
				globalId++, c_max, ir, e, single, atomicMap, nodeFromT, nodeToT);

	}

	~SingleCreditEdge(){
		delete credit_remain;
		for (auto it : debt_current){
			delete it.second;
		}
	}

	bool routeCredit(int current, double interest_rate, int& globalId, Edge* e, 
		int single, unordered_map<int, AtomicEdge*>& atomicMap, Node* nodeFromT, Node* nodeToT){


		if (credit_remain->capacity < current){
			return false;
		}

		if (debt_current.find(interest_rate) != debt_current.end()){

			debt_current[interest_rate]->capacity += current;

		} else {
			
			debt_current[interest_rate] = new AtomicEdge(1, 
				globalId++, current, interest_rate, e, single, atomicMap, nodeFromT, nodeToT);

		}

		credit_remain->capacity -= current;
		return true;
	}

	bool routeDebt(int current, double interest_rate) {

		if (debt_current.find(interest_rate) == debt_current.end()) {
			return false;
		}
		if (debt_current.find(interest_rate)->second->capacity < current){
			return false;
		}

		debt_current.find(interest_rate)->second->capacity -= current;
		credit_remain->capacity += current;
		return true;
	}

	void print(){
		cout << "  single credit edge, capacity: " << credit_max 
		<< " ir: " << credit_interest_rate << endl;
		credit_remain->print();
		for (auto it : debt_current){
			it.second->print();
		}
	}
};



class Edge{

public:

	vector<int> edgeUsage;
	
	Node* nodeFrom;
	Node* nodeTo;
	vector <SingleCreditEdge*> singleCreditEdges;

	Edge(Node* nodeFromT, Node* nodeToT);
	Edge(const Edge& e, Node* nodeFromT, Node* nodeToT, unordered_map<int, AtomicEdge*>& atomicMap);
	~Edge();

	SingleCreditEdge* addSingleCreditEdge(double interest_rate, int cap, 
		int& atomicGlobalId, unordered_map<int, AtomicEdge*>& atomicMap);

	void routeAtomicEdge(AtomicEdge* a, int flow, double interest_rate, 
		int& atomicGlobalId, unordered_map<int, AtomicEdge*>& atomicMap, int transSeqNum);

	void print();

};


#endif