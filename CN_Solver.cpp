#include "CN_Solver.h"

void LpSolver::buildLpProblem(CplexConverter& cplexConverter)
{
	IloEnv   env;
	try {
		IloModel model(env);
		IloNumVarArray var(env);
		IloRangeArray con(env);
		this->populatebyrow (cplexConverter, model, var, con); //modify where the graph is located
		IloCplex cplex(model);

		cout << "before solving " << endl;
		cout << var << endl;
		cout << con << endl;


		// Optimize the problem and obtain solution.
		if ( !cplex.solve() ) {
		   env.error() << "Failed to optimize LP" << endl;
		   throw(-1);
		}


		IloNumArray vals(env);
		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value  = " << cplex.getObjValue() << endl;
		cplex.getValues(vals, var);
		env.out() << "Values        = " << vals << endl;
		cplex.getSlacks(vals, con);
		env.out() << "Slacks        = " << vals << endl;
		cplex.getDuals(vals, con);
		env.out() << "Duals         = " << vals << endl;
		cplex.getReducedCosts(vals, var);
		env.out() << "Reduced Costs = " << vals << endl;
    	cplex.exportModel("lpex1.lp");

		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			cplexConverter.results.push_back(cplex.getValue(var[i]));
		}

		cout << "after solving " << endl;
		cout << var << endl;
		cout << con << endl;


	}
	catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		cerr << "Unknown exception caught" << endl;
	}
	env.end();

	return;
}  
	
void LpSolver::populatebyrow (CplexConverter& cplexConverter, 
	IloModel model, IloNumVarArray x, IloRangeArray c)
{
	IloEnv env = model.getEnv();
	// CAPITAL LETTERS MEAN I NEED YOUR HELP, here is help 

	IloExpr cost(env);
	
	// Create Variables
	cout << "size of var: " << cplexConverter.variables.size() << endl;
	for (int i = 0; i < cplexConverter.variables.size(); ++i){
		IloNumVar iloVar(env, 0.0, cplexConverter.capacities[i]);
		// cout << iloVar << endl;
		x.add(iloVar);
	}
	for (int i = 0; i < cplexConverter.variables.size(); ++i)
	{
		cout << "xi: " << x[i] << endl;
	}


	//Capacity Constraints
	for (auto &it : cplexConverter.atomicIdToVarIdDict){
		IloExpr t(env);
		cout << "adding constraint ";
		for (int j = 0; j < it.second.size(); j++){
			cout << "x[" << it.second[j] << "] + ";
			t += x[it.second[j]];
		}
		cout << endl;
		c.add(t <= cplexConverter.graph->atomicEdges[it.first]->capacity);
		// cout << c << endl;
		t.end();
	}

	// other constraints
	for (auto nodePair : cplexConverter.graph->nodes){

		// For all nodes
		Node* n = nodePair.second;

		if(n == cplexConverter.src){

			// source constraints
			IloExpr inFlow(env);
			IloExpr outFlow(env);	
			for(auto &atoIn : n->atomicEdge_in){
				int aeId = atoIn.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					outFlow += x[vId];
					cost += cplexConverter.graph->atomicEdges[cplexConverter.variables[vId].atomicEdgeId]->interest_rate * inFlow;
				}
			}
			for (auto &atoOut : n->atomicEdge_out){
				int aeId = atoOut.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					inFlow += x[vId];
					cost -= cplexConverter.graph->atomicEdges[cplexConverter.variables[vId].atomicEdgeId]->interest_rate * outFlow;
				}
			}

			c.add(outFlow - inFlow == cplexConverter.request);
			inFlow.end();
			outFlow.end();

		} else if(n == cplexConverter.dest){

			// destination constraints
			IloExpr inFlow(env);
			IloExpr outFlow(env);	
			for(auto &atoIn : n->atomicEdge_in){
				int aeId = atoIn.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					outFlow += x[vId];
				}
			}
			for (auto &atoOut : n->atomicEdge_out){
				int aeId = atoOut.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					inFlow += x[vId];
				}
			}

			c.add(inFlow - outFlow == cplexConverter.request);
			inFlow.end();
			outFlow.end();
		} else {

			// Monotonicity Constraints
			for (int i = 0; i < credNetConstants.totalIrs.size(); ++i){
				IloExpr tempin(env);
				IloExpr tempout(env);

				for (auto &atoIn : n->atomicEdge_in){
					int aeId = atoIn.second->atomicEdgeId;
					for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){

						// var Id
						int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
						if (cplexConverter.variables[vId].interest_rate <= credNetConstants.totalIrs[i]){
							tempout += x[vId];
						}
					}
				}
				for (auto &atoOut : n->atomicEdge_out){
					int aeId = atoOut.second->atomicEdgeId;
					for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){

						// var Id
						int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
						if (cplexConverter.variables[vId].interest_rate <= credNetConstants.totalIrs[i]){
							tempin += x[vId];
						}
					}
				}

				c.add(tempout - tempin >= 0);
				tempout.end();
				tempin.end();
			}

			//Flow Constraints
			IloExpr inFlow(env);
			IloExpr outFlow(env);	
			for(auto &atoIn : n->atomicEdge_in){
				int aeId = atoIn.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					outFlow += x[vId];
				}
			}
			for (auto &atoOut : n->atomicEdge_out){
				int aeId = atoOut.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					inFlow += x[vId];
				}
			}

			c.add(inFlow - outFlow == 0);
			inFlow.end();
			outFlow.end();

		}

	}

	model.add(c);
	model.add(IloMinimize(env, 1));
	// model.add(IloMaximize(env,cost));  //option to minimize cost
	cost.end();

}  // END populatebyrow

