#ifndef CN_CreditNet
#define CN_CreditNet

#include <unordered_map>
#include <list>
#include <string>
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_Solver.h"

class CreditNet : public Graph{
private:

public:
	CreditNet(int finNumT);
	CreditNet();

	// Inter Bank Trans
	// liquidity test
	int genInterBankTrans(int request, string mode);

	
	void printPayoff();
};


#endif