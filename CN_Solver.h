#ifndef CN_Solver
#define CN_Solver

#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_Constants.h"
#include "CN_CplexConverter.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <ilcplex/ilocplex.h>
  ILOSTLBEGIN

class LpSolver{
public:
	bool solveLpProblem(CplexConverter& cplexConverter, string mode);
	
	void populatebyrow(CplexConverter& cplexConverter, 
		IloModel model, IloNumVarArray x, IloRangeArray c);

	void addObjective(string mode, CplexConverter& cplexConverter, IloModel model, 
	IloNumVarArray x, IloRangeArray c);
};


#endif
