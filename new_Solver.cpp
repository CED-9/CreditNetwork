#include <ilcplex/ilocplex.h>
  ILOSTLBEGIN
  

class LpSolver{
public:
	void buildLpProblem(double req, Node* src, Node* dest, Graph* g);
	
	void populatebyrow (Graph g, IloModel model, IloNumVarArray x, 
	IloRangeArray c, unordered_map <int, pair <int,double> > fakeEdgeDict, 
	unordered_map <int, vector <pair<int,double> > > edgeDict, double req);


};


LpSolver::buildLpProblem(double req, Node* src, Node* dest, Graph* g){
	IloEnv   env;
	try {
		IloModel model(env);
		unordered_map <int, pair <int,double> > fakeEdgeDict;
		unordered_map <int, vector <pair<int,double> > > edgeDict;
		IloNumVarArray var(env);
		IloRangeArray con(env);
		this->populatebyrow (this->Graph, model, var, con,fakeEdgeDict,edgeDict,req); //modify where the graph is located
		IloCplex cplex(model);

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
    	// cplex.exportModel("lpex1.lp");
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
	
void LpSolver::populatebyrow (Graph g, IloModel model, IloNumVarArray x, 
	IloRangeArray c, unordered_map <int, pair <int,double> > fakeEdgeDict, 
	unordered_map <int, vector <pair<int,double> > > edgeDict, double req)
{
	IloEnv env = model.getEnv();
	//CAPITAL LETTERS MEAN I NEED YOUR HELP, here is help 
	//Create Variables
	int id = 0;
	for (auto &it : g->atomicEdges){
		AtomicEdge* at = it.second;
		if (!at->isDebt){
	 		for(auto ir : g->allInterests){
		 		if (at->interest_rate <= ir){
		 			x.add(IloNumVar(env, 0.0, IloInfinity));
					fakeEdgeDict[id].first = at->atomicEdgeId;
					fakeEdgeDict[id].second = ir;
					pair<int, double> tmpPair;
					tmpPair.first = id;
					tmpPair.second = ir;
					//make sure these are copied and not referenced
					edgeDict[at->atomicEdgeId].push_back(tmpPair);
					id += 1;
				}	
	 		}
		}
		else{
			x.add(IloNumVar(env, 0.0, IloInfinity));
			fakeEdgeDict[id].first = at->atomicEdgeId;
			fakeEdgeDict[id].second = at->interest_rate;
			pair<int, double> tmpPair;
			tmpPair.first = id;
			tmpPair.second = at->interest_rate;
			//make sure these are copied and not referenced
			edgeDict[at->atomicEdgeId].push_back(tmpPair);
			id += 1;
		}
	}


	//Capacity Constraints
	for (auto &it : g->atomicEdges){
		IloExpr t(env);	
		for (int j = 0; j < edgeDict[i].size(); j++){
			t += x[edgeDict[i][j].first];      //not sure if right syntax for accessing vector element
		}
		c.add(t <= it.second->capacity);
		t.end();
	}

	for (auto nodePair : nodes){
		//Monotonicity Constraints
		Node* n = nodePair.second;
		if(n != src){	
			for (auto& possibleIr : g->allInterests){
				IloExpr tempin(env);
				IloExpr tempout(env);

				for (auto& edgePair : n->edge_out){
					// Edge
					Edge* e = edgePair.second;
					for (int i = 0; i < e->singleCreditEdges.size(); ++i){
						// Single Credit Edge
						SingleCreditEdge* se = e->singleCreditEdges[i];
						for (auto atomicEdgePair : se->debt_current){
							// Atomic Edge
							AtomicEdge* ae = atomicEdgePair.second;
							for (int j = 0; j < edgeDict[ae.atomicEdgeId].size(); j++){
								if (edgeDict[ae.atomicEdgeId][j].second <= R){
									tempin += x[edgeDict[ae.atomicEdgeId][j].first];
								}
							}
						}
					}
				}

				for (auto& edgePair : n->edge_in){
					// Edge
					Edge* e = edgePair.second;
					for (int i = 0; i < e->singleCreditEdges.size(); ++i){
						// Single Credit Edge
						SingleCreditEdge* se = e->singleCreditEdges[i];
						
						// Atomic Edge
						AtomicEdge* ae = se->credit_remain;
						for (int j = 0; j < edgeDict[ae.atomicEdgeId].size(); j++){
							if (edgeDict[ae.atomicEdgeId][j].second <= R){
								tempin += x[edgeDict[ae.atomicEdgeId][j].first];
							}
						}
						
					}
				}




				for(EACH INCOMING ATOMIC EDGE E_IN){ //Incoming means flow into node i
					for (int j = 0; j < edgeDict[E_IN.id].size(); j++){
						if (edgeDict[E_IN.id][j].second <= R){
							tempin += x[edgeDict[E_IN.id][j].first];
						}
					}
				}
				for(EACH OUTGOING ATOMIC EDGE E_OUT){ //Outgoing means flow out of node i
					for (int j = 0; j < edgeDict[E_OUT.id].size(); j++){
						if (edgeDict[E_OUT.id][j].second <= R){
							tempout += x[edgeDict[E_OUT.id][j].first];
						}
					}
				}
				c.add(tempin <= tempout);
				tempout.end();
				tempin.end();
			}
			//Flow Constraints
			IloExpr inFlow(env);
			IloExpr outFlow(env);	
			for(EACH INCOMING ATOMIC EDGE E_IN){
				for (int j = 0; j < edgeDict[E_IN.id].size(); j++){
					inFlow += x[edgeDict[E_IN.id][j].first];
				}
			}
			for(EACH OUTGOING ATOMIC EDGE E_OUT){
				for (int j = 0; j < edgeDict[E_OUT.id].size(); j++){
					outFlow += x[edgeDict[E_OUT.id][j].first];					
				}
			}

			c.add(inflow <= outflow)	;
			c.add(inflow >= outflow)	;
			inflow.end();
			outflow.end();
			}
		//Flow from Source
		if(nodes[i] == SOURCE){
			IloExpr s(env);
			IloExpr cost(env);			
			for(EACH OUTGOING ATOMIC EDGE E_OUT){
				for (int j = 0; j < edgeDict[E_OUT.id].size(); j++){
					s += x[edgeDict[E_OUT.id][j].first];	
					//Also calculate cost to source as objective
					cost += x[edgeDict[E_OUT.id][j].first]*edgeDict[E_OUT.id][j].second;
				}
			}
			c.add(s <= req);
			c.add(s >= req);
			s.end();
		}
		//Flow to Sink
		if(nodes[i] == SINK){
			IloExpr s(env);
			for(EACH INCOMING ATOMIC EDGE E_IN){
				for (int j = 0; j < edgeDict[E_IN.id].size(); j++){
					s += x[edgeDict[E_IN.id][j].first];					
				}
			}
			c.add(s <= req);
			c.add(s >= req);
			s.end();
		}
	}
	model.add(c);
	model.add(IloMaximize(env,1));
	// model.add(IloMaximize(env,cost));  //option to minimize cost
	cost.end();

}  // END populatebyrow
  
  //Assume we have solutions x[...] and fakeEdgeDictionary. Then this is how we copy back:
 
 void copyback(Graph g, Solution x, map fakeEdgeDictionary){
	 for each x[i] in x:
		route amount x on atomic edge fakeEdgeDictionary[i].first at interest rate fakeEdgeDictionary[i].second;
 }