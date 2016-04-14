
#include "CN_GraphModifier.h"


////////////////////////////////////////////////////////
// Edge Modifications
// Add, Remove, Modify
////////////////////////////////////////////////////////

void GraphModifier::setRoutePreference(vector<string> &v, Graph& g){

	for (int i = 0; i < g.nodes.size(); i++) {
		g.nodes[i]->routePreference = v[i];
	}

	return;
}


bool GraphModifier::routeAtomicEdge(Graph* g, AtomicEdge* at, int amt, double ir){

	SingleCreditEdge* sc = at->originSingleCreditEdge;

	
	if (at->isDebt){
		// route on debt
		if (sc->debt_current.find(ir) == sc->debt_current.end() ){
			return false;
		}
		if (sc->debt_current[ir]->capacity < amt){
			return false;
		}

		sc->debt_current[ir]->capacity -= amt;
		sc->credit_remain->capacity += amt;

	} else {
		// route on credit
		if (at->capacity < amt){
			return false;
		}

		if (sc->debt_current.find(ir) != sc->debt_current.end()){
			// ir already exists
			sc->debt_current[ir]->capacity += amt;
		} else {
			// new ir debt
			AtomicEdge* newAtomicEdge = new AtomicEdge(true, 
				g->atomicGlobalId, amt, ir, at->originEdge, sc, 
				at->nodeFrom, at->nodeTo);
			sc->debt_current[ir] = newAtomicEdge;
			g->atomicEdges[g->atomicGlobalId] = newAtomicEdge;
			g->atomicGlobalId++;
		}

		at->capacity -= amt;
	}

	return true;
}

void GraphModifier::addEdge(Graph* g, Node* nodeFrom, Node* nodeTo, 
	double credit_ir, double debt_ir, int currDebt, int cap){

	// if there was no connection
	Edge* e;
	if (nodeTo->edge_in.find(nodeFrom->nodeId) == nodeTo->edge_in.end() ){
		e = new Edge(nodeFrom, nodeTo);
		nodeTo->edge_in[nodeFrom->nodeId] = e;
		nodeFrom->edge_out[nodeTo->nodeId] = e;

		g->edges.push_back(e);
	} else {
		e = nodeTo->edge_in[nodeFrom->nodeId];
	}

	// create new single credit edge, new atomic edge
	SingleCreditEdge* temp = new SingleCreditEdge(
		e->singleCreditEdges.size(), cap, 
		credit_ir, g->atomicGlobalId, e, nodeFrom, nodeTo);
	e->singleCreditEdges.push_back(temp);
	AtomicEdge* at = temp->credit_remain;
	g->atomicEdges[g->atomicGlobalId] = at;
	g->atomicGlobalId++;

	// if debt exists
	if (currDebt != 0){
		GraphModifier::routeAtomicEdge(g, at, currDebt, debt_ir);
	}

}

void GraphModifier::updateNodeDegrees(Graph* g){
	for (auto& it : g->nodes){
		it.second->degree = it.second->edge_in.size();
	}
}


////////////////////////////////////////////////////////
// Statistics
// Node Degree
// Node banlance
//
////////////////////////////////////////////////////////


double GraphModifier::getAvgAtomicIouEdges(Graph* g){
	int cnt = 0;
	double totalIrCap = 0;
	double totalCap = 0;

	for (auto &it : g->atomicEdges){
		if (!it.second->isDebt || it.second->capacity == 0){
			continue;
		}
		totalIrCap += it.second->capacity * it.second->interest_rate;
		totalCap += it.second->capacity;
		cnt ++;
	}

	return totalIrCap / totalCap;
}

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

////////////////////////////////////////////////////////
// Print 
// AtomicEdge, SingleCreditEdge, Edge, Node, Graph
////////////////////////////////////////////////////////

void GraphModifier::print(SingleCreditEdge* sc){
	cout << "  single credit edge, capacity: " << sc->contract_credit 
	<< " ir: " << sc->contract_interest_rate << endl;
	
	GraphModifier::print(sc->credit_remain);

	for (auto it : sc->debt_current){
		GraphModifier::print(it.second);
	}
}


void GraphModifier::print(AtomicEdge* at){
	cout << "    Atomic Edge, id: " << at->atomicEdgeId << " " 
	<< "capacity: " << at->capacity 
	<< " ir: " << at->interest_rate 
	<< " isDebt: " << at->isDebt 
	<< " single index " << at->originSingleCreditEdge->indexOfEdge
	<< " from " << at->nodeFrom->nodeId 
	<< " to " << at->nodeTo->nodeId
	<< endl;
}


void GraphModifier::print(Edge* e){
	cout << "From Node " << e->nodeFrom->nodeId 
	<< " To Node " << e->nodeTo->nodeId << endl;

	for (int i = 0; i < e->singleCreditEdges.size(); ++i){
		GraphModifier::print(e->singleCreditEdges[i]);
	}
	
}

void GraphModifier::print(Node* n){
	cout << "Node " << n->nodeId << endl;
	for (auto &it : n->edge_in){
		GraphModifier::print(it.second);
	}
	for (auto &it : n->edge_out){
		GraphModifier::print(it.second);
	}

	cout << "Atomic Edges: " << endl;
	for (auto &it : n->atomicEdge_in){
		GraphModifier::print(it.second);
	}
	for (auto &it : n->atomicEdge_out){
		GraphModifier::print(it.second);
	}
	cout << endl;

	return;
}

void GraphModifier::print(Graph* g){
	for (int i = 0; i < g->nodes.size(); ++i){
		GraphModifier::print(g->nodes[i]);
	}
}