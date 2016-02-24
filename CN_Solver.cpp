#include "CN_Solver.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

extern CredNetConstants credNetConstants;


bool LpSolver::solveLpProblem(CplexConverter& cplexConverter, string mode)
{
	// clock_t t;
	// t = clock();
	
	IloEnv   env;
	bool success = false;
	try {
		IloModel model(env);
		IloNumVarArray var(env);
		IloRangeArray con(env);
		this->populatebyrow(cplexConverter, model, var, con); //modify where the graph is located
		this->addObjective(mode, cplexConverter, model, var, con);
		IloCplex cplex(model);

		// cout << "before solving " << endl;
		// cout << var << endl;
		// cout << con << endl;

		cplex.setOut(env.getNullStream());
		// Optimize the problem and obtain solution.
		if ( !cplex.solve() ) {
		   // env.error() << "Failed to optimize LP" << endl;
		   throw(-1);
		}

		IloNumArray vals(env);
		
		// env.out() << "Solution status = " << cplex.getStatus() << endl;
		// env.out() << "Solution value  = " << cplex.getObjValue() << endl;
		// cplex.getValues(vals, var);
		// env.out() << "Values        = " << vals << endl;
		// cplex.getSlacks(vals, con);
		// env.out() << "Slacks        = " << vals << endl;
		// cplex.getDuals(vals, con);
		// env.out() << "Duals         = " << vals << endl;
		// cplex.getReducedCosts(vals, var);
		// env.out() << "Reduced Costs = " << vals << endl;
		
    	// cplex.exportModel("lpex1.lp");

		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			cout << "results from LP: " << cplex.getValue(var[i]) << endl;
			cplexConverter.results.push_back(cplex.getValue(var[i]));
		}

		// cout << "after solving " << endl;
		// cout << var << endl;
		// cout << con << endl;
		success = true;

	}
	catch (IloException& e) {
		// cerr << "Concert exception caught: " << e << endl;
	}
	catch (...) {
		// cerr << "Unknown exception caught" << endl;
	}
	env.end();


	// t = clock() - t;
	// printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);

	return success;
}

void LpSolver::addObjective(string mode, 
	CplexConverter& cplexConverter, IloModel model, 
	IloNumVarArray x, IloRangeArray c){
	
	// cerr << mode << endl;

	IloEnv env = model.getEnv();

	if (mode == "MIN_SRC_COST"){

		IloExpr cost(env);

		// add cost to all atomic edges
		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			cost += x[i] * cplexConverter.variables[i].interest_rate;
		}

		for(auto &atoIn : cplexConverter.src->atomicEdge_in){
			int aeId = atoIn.second->atomicEdgeId;
			for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
				// var Id
				int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
				cost += cplexConverter.variables[vId].interest_rate * x[vId];

				// cout << "adding " << cplexConverter.variables[vId].interest_rate 
				// 		<< " * " << vId << endl;
			}
		}
		model.add(IloMinimize(env, cost));

	} else if (mode == "MIN_CREDIT_COST") {

		IloExpr cost(env);

		// add cost to all atomic edges
		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			cost += x[i] * cplexConverter.variables[i].interest_rate;
		}

		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			int aeId = cplexConverter.variables[i].atomicEdgeId;
			if (!cplexConverter.graph->atomicEdges[aeId]->isDebt){
				cost += x[i];
			}
		}
		model.add(IloMinimize(env, cost));

	} else if (mode == "MIN_CREDIT_SRC") {

		IloExpr cost(env);

		for(auto &atoIn : cplexConverter.src->atomicEdge_in){
			int aeId = atoIn.second->atomicEdgeId;
			for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
				// var Id
				int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
				cost += cplexConverter.variables[vId].interest_rate * x[vId];
				if (!cplexConverter.graph->atomicEdges[aeId]->isDebt){
					cost += x[vId];
				}			

				// cout << "adding " << cplexConverter.variables[vId].interest_rate 
				// 		<< " * " << vId << endl;
			}
		}
		model.add(IloMinimize(env, cost));

	}	else if (mode == "MIN_DEGREE_SRC") {

		IloExpr cost(env);

		for(auto &atoIn : cplexConverter.src->atomicEdge_in){
			int aeId = atoIn.second->atomicEdgeId;
			AtomicEdge* atEdge = cplexConverter.graph->atomicEdges[aeId];			
			for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
				// var Id
				int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
				cost += cplexConverter.variables[vId].interest_rate * x[vId];					
				if (!atEdge->isDebt){
					cost += atEdge->nodeTo->degree * x[vId];
				} else {
					cost += atEdge->nodeFrom->degree * x[vId];
				}
				// cout << "adding " << cplexConverter.variables[vId].interest_rate 
				// 		<< " * " << vId << endl;
			}
		}
		model.add(IloMinimize(env, cost));

	}	
		else if (mode == "MAX_IR_COST") {

		IloExpr cost(env);
		// add cost to all atomic edges
		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			cost += 100 - x[i] * cplexConverter.variables[i].interest_rate;
		}
		model.add(IloMinimize(env, cost));

	}	else if (mode == "MIN_SUMIR_COST") {

		IloExpr cost(env);
		// add cost to all atomic edges
		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			cost += x[i] * cplexConverter.variables[i].interest_rate;
		}
		model.add(IloMinimize(env, cost));

	} 	else if (mode == "MIN_DEGREE_COST") {

		IloExpr cost(env);

		// add cost to all atomic edges
		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			cost += x[i] * cplexConverter.variables[i].interest_rate;
		}
		
		for (int i = 0; i < cplexConverter.variables.size(); ++i){
			int aeId = cplexConverter.variables[i].atomicEdgeId;
			AtomicEdge* atEdge = cplexConverter.graph->atomicEdges[aeId];
			
			for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
				// var Id
				int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
				if (!atEdge->isDebt){
					cost += atEdge->nodeTo->degree * x[vId];
				} else {
					cost += atEdge->nodeFrom->degree * x[vId];
				}	
			}

		}
		model.add(IloMinimize(env, cost));

	} else {
		// default
		model.add(IloMinimize(env, 1));
	}


}
	
void LpSolver::populatebyrow (CplexConverter& cplexConverter, 
	IloModel model, IloNumVarArray x, IloRangeArray c)
{
	IloEnv env = model.getEnv();
	// CAPITAL LETTERS MEAN I NEED YOUR HELP, here is help 

	// IloExpr cost(env);
	
	// Create Variables
	// cout << "size of var: " << cplexConverter.variables.size() << endl;
	for (int i = 0; i < cplexConverter.variables.size(); ++i){
		IloNumVar iloVar(env, 0.0, cplexConverter.capacities[i], IloNumVar::Int);
		// cout << iloVar << endl;
		x.add(iloVar);
	}

	//Capacity Constraints
	for (auto &it : cplexConverter.atomicIdToVarIdDict){
		IloExpr t(env);
		// cout << "adding constraint ";
		for (int j = 0; j < it.second.size(); j++){
			// cout << "x[" << it.second[j] << "] + ";
			t += x[it.second[j]];
		}
		// cout << endl;
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
			// IloExpr inFlow(env);
			IloExpr outFlow(env);	
			for(auto &atoIn : n->atomicEdge_in){
				int aeId = atoIn.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					outFlow += x[vId];
					// cost += cplexConverter.graph->atomicEdges[cplexConverter.variables[vId].atomicEdgeId]->interest_rate * x[vId];
				}
			}
			for (auto &atoOut : n->atomicEdge_out){
				int aeId = atoOut.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					// inFlow += x[vId];
					c.add(x[vId] == 0);
					// cost -= cplexConverter.graph->atomicEdges[cplexConverter.variables[vId].atomicEdgeId]->interest_rate * x[vId];
				}
			}

			c.add(outFlow == cplexConverter.request);
			// inFlow.end();
			outFlow.end();

		} else if(n == cplexConverter.dest){

			// destination constraints
			IloExpr inFlow(env);
			// IloExpr outFlow(env);
			for(auto &atoIn : n->atomicEdge_in){
				int aeId = atoIn.second->atomicEdgeId;
				for (int j = 0; j < cplexConverter.atomicIdToVarIdDict[aeId].size(); j++){
					// var Id
					int vId = cplexConverter.atomicIdToVarIdDict[aeId][j];
					// outFlow += x[vId];
					c.add(x[vId] == 0);
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

			c.add(inFlow == cplexConverter.request);
			inFlow.end();
			// outFlow.end();

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
	// model.add(IloMinimize(env, cost));
	// model.add(IloMaximize(env,cost));  //option to minimize cost
	// cost.end();

}  // END populatebyrow

