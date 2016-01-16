#ifndef CN_Solver
#define CN_Solver

#include "CN_WidgetGraph.h"
#include "CN_Constants.h"

class CplexConverter{

public:

	struct Variable{
		int varId;
		int atomicEdgeId;
		double interest_rate;
		Variable(int v, int a, double ir) : varId(v), atomicEdgeId(a), interest_rate(ir) {}
	};

	vector<Variable> variables;
	unordered_map < int, vector<int> > atomicIdToVarIdDict;
	unordered_map < int, int > varIdToAtomicIdDict;
	vector<double> capacities;
	vector<double> results;
	double request;
	Node* src;
	Node* dest;
	Graph* graph;
	
	void constructCplex(Graph* g, Node* s, Node* t, int req){
		graph = g;
		src = s;
		dest = t;
		request = req;

		int globalVarId = 0;
		for (int i = 0; i < g->atomicEdges.size(); i++){
			AtomicEdge* at = g->atomicEdges[i];
			if (at->isDebt){
				
				int tempId = globalVarId++;
				cout << "adding: " << tempId << " " << at->atomicEdgeId << " " << at->interest_rate << endl;
				Variable v(tempId, at->atomicEdgeId, at->interest_rate);
				variables.push_back(v);
				capacities.push_back(at->capacity);
				atomicIdToVarIdDict[at->atomicEdgeId].push_back(tempId);
				varIdToAtomicIdDict[tempId] = at->atomicEdgeId;

			} else {
				for (int i = 0; i < credNetConstants.totalIrs.size(); ++i){
					if (credNetConstants.totalIrs[i] >= at->interest_rate){

						int tempId = globalVarId++;
						cout << "adding: " << tempId << " " << at->atomicEdgeId << " " << at->interest_rate << endl;
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

	void printInput(){
		for (int i = 0; i < variables.size(); ++i){
			cout << "Var ID " << variables[i].varId 
			<< ", Atomic Edge ID " << variables[i].atomicEdgeId
			<< ", IR " << variables[i].interest_rate 
			<< ", range 0 - " << capacities[i] << endl;
		}
	}

	void printResult(){
		for (int i = 0; i < results.size(); ++i){
			cout << results[i] << " ";
		}
		cout << endl;
	}

	void copyBack(){
		for (int i = 0; i < variables.size(); ++i){
			int id = variables[i].atomicEdgeId;
			if (results[i] == 0){
				continue;
			}
			this->graph->atomicEdges[id]
				->route(results[i], variables[i].interest_rate, this->graph);
		}
	}
};


#include <ilcplex/ilocplex.h>
  ILOSTLBEGIN

class LpSolver{
public:
	void buildLpProblem(CplexConverter& cplexConverter);
	
	void populatebyrow (CplexConverter& cplexConverter, 
		IloModel model, IloNumVarArray x, IloRangeArray c);

};


#endif
