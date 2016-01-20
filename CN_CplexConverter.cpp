#include "CN_CplexConverter.h"

extern CredNetConstants credNetConstants;


void CplexConverter::constructCplex(Graph* g, Node* s, Node* t, int req){
	graph = g;
	src = s;
	dest = t;
	request = req;

	int globalVarId = 0;
	for (int i = 0; i < g->atomicEdges.size(); i++){
		AtomicEdge* at = g->atomicEdges[i];
		if (at->isDebt){
			
			int tempId = globalVarId++;
			// cout << "adding: " << tempId << " " << at->atomicEdgeId << " " << at->interest_rate << endl;
			Variable v(tempId, at->atomicEdgeId, at->interest_rate);
			variables.push_back(v);
			capacities.push_back(at->capacity);
			atomicIdToVarIdDict[at->atomicEdgeId].push_back(tempId);
			varIdToAtomicIdDict[tempId] = at->atomicEdgeId;

		} else {
			for (int i = 0; i < credNetConstants.totalIrs.size(); ++i){
				if (credNetConstants.totalIrs[i] >= at->interest_rate){

					int tempId = globalVarId++;
					// cout << "adding: " << tempId << " " << at->atomicEdgeId << " " << at->interest_rate << endl;
					Variable v(tempId, at->atomicEdgeId, credNetConstants.totalIrs[i]);
					variables.push_back(v);
					capacities.push_back(at->capacity);
					atomicIdToVarIdDict[at->atomicEdgeId].push_back(tempId);
					varIdToAtomicIdDict[tempId] = at->atomicEdgeId;

				}
			}
		}
	}
}

void CplexConverter::printInput(){
	for (int i = 0; i < variables.size(); ++i){
		cout << "Var ID " << variables[i].varId 
		<< ", Atomic Edge ID " << variables[i].atomicEdgeId
		<< ", IR " << variables[i].interest_rate 
		<< ", range 0 - " << capacities[i] << endl;
	}
}

void CplexConverter::printResult(){
	for (int i = 0; i < results.size(); ++i){
		cout << results[i] << " ";
	}
	cout << endl;
}

void CplexConverter::copyBack(){
	for (int i = 0; i < variables.size(); ++i){
		int id = variables[i].atomicEdgeId;
		if (results[i] == 0){
			continue;
		}
		this->graph->atomicEdges[id]
			->route(results[i], variables[i].interest_rate, this->graph);
	}
}