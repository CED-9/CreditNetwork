
#include "CN_GraphModifier.h"


double GraphModifier::getNodeCurrBanlance(Node* n){
	double temp = 0;
	for (auto& it : n->atomicEdge_in){
		if (it.second->isDebt){
			temp += it.second->capacity * it.second->interest_rate;
		}
	}
	for (auto& it : n->atomicEdge_out){
		if (it.second->isDebt){
			temp -= it.second->capacity * it.second->interest_rate;
		}
	}
	return temp;
}



void GraphModifier::print(SingleCreditEdge* sc){
	cout << "  single credit edge, capacity: " << sc->contract_credit 
	<< " ir: " << sc->contract_interest_rate << endl;
	
	this->print(credit_remain);

	for (auto it : debt_current){
		this->print(it.second);
	}
}


void GraphModifier::print(AtomicEdge* at){
	cout << "    Atomic Edge, id: " << at->atomicEdgeId << " " 
	<< "capacity: " << at->capacity 
	<< " ir: " << at->interest_rate 
	<< " isDebt: " << at->isDebt 
	<< " single index " << at->singleCreditIndex 
	<< " from " << at->nodeFrom->nodeId 
	<< " to " << at->nodeTo->nodeId
	<< endl;
}


void GraphModifier::print(Edge* e){
	cout << "From Node " << e->nodeFrom->getNodeId() 
	<< " To Node " << e->nodeTo->getNodeId() << endl;

	for (int i = 0; i < e->singleCreditEdges.size(); ++i){
		this->print(e->singleCreditEdges[i]);
	}
	
}


void GraphModifier::print(Node* n){
	cout << "Node " << n->nodeId << endl;
	for (auto &it : n->edge_in){
		this->print(it.second);
	}
	for (auto &it : n->edge_out){
		this->print(it.second);
	}

	cout << "Atomic Edges: " << endl;
	for (auto &it : n->atomicEdge_in){
		this->print(it.second);
	}
	for (auto &it : n->atomicEdge_out){
		this->print(it.second);
	}
	cout << endl;

	return;
}


void GraphModifier::print(Graph* n);