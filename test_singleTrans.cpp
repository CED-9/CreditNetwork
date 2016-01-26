#include "CN_Constants.h"
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_Solver.h"
#include "CN_CreditNet.h"

#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <mutex>
#include <thread>
#include <cmath>

using namespace std;

extern CredNetConstants credNetConstants;

int main(){

	credNetConstants.addIr(0.01);
	credNetConstants.addIr(0.02);
	credNetConstants.addIr(0.03);
	credNetConstants.addIr(0.04);

	credNetConstants.print();


	CreditNet creditNet(200);
	creditNet.genTest0Graph(0.025, 4, 1);
	for (int i = 0; i < 100; ++i){
		cout << "result: " << creditNet.genInterBankTrans(1, "MIN_CREDIT_COST") << endl;
	}

	ofstream fout("out");
	creditNet.printAtomicIouEdges(fout);
	fout.close();

	return 0;
}