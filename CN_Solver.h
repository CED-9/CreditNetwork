#ifndef CN_Solver
#define CN_Solver

#include "CN_WidgetGraph.h"
#include "CN_Constants.h"
#include "CN_CplexConverter.h"

#include <ilcplex/ilocplex.h>
  ILOSTLBEGIN

class LpSolver{
public:
	bool solveLpProblem(CplexConverter& cplexConverter);
	
	void populatebyrow (CplexConverter& cplexConverter, 
		IloModel model, IloNumVarArray x, IloRangeArray c);

};


#endif
