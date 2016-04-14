#ifndef CN_CreditNet
#define CN_CreditNet

#include <unordered_map>
#include <list>
#include <string>
#include "CN_Graph.h"
#include "CN_Solver.h"
#include "CN_FinNode.h"

class CreditNet : public Graph{
private:

public:

	CreditNet(int finNumT);
	CreditNet();


	// Cred nets operations
	bool issueCredit(Node* NodeFrom, Node* NodeTo, int cap, double ir);
	bool revokeCredit(AtomicEdge* at, int amt);
	bool paymentRouting(Node* NodeFrom, Node* NodeTo, string mode, 
		int amt, const set<Node*>& excludedNodes, transSeqNum = -1);
	// combo operations
	bool retireDebt(AtomicEdge* at, int amt);
	bool makeDeposit(HouseHoldNode* h, FinNode* f, int amt, double ir);
	bool wiredPaymentRouting(HouseHoldNode* hFrom, 
		FinNode* fTo, HouseHoldNode* hTo, string mode, 
		int amt, set<Node*> excludedNodes, transSeqNum = -1);


	bool dayEndClearing();


	// Inter Bank Trans
	// liquidity test
	int genInterBankTrans(int request, string mode, int transSeqNum);
	void printPayoff();
	////////// Generate Test Credit Net /////////////////////////////
	void genTestCredNet100();

};


#endif