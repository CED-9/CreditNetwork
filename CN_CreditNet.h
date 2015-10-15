#ifndef CN_CreditNet
#define CN_CreditNet

#include <unordered_map>
#include <list>
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_Searcher.h"
#include "CN_Executer.h"

class CreditNet : public Graph{
private:

public:
	CreditNet(int finNumT);

	// Inter Bank Trans
	// liquidity test
	int genInterBankTrans();

	
	void printPayoff();
};


#endif