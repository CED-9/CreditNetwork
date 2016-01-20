
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_CreditNet.h"
#include "CN_Solver.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <random>
#include <mutex>
#include <thread>
#include <cmath>

using namespace std;

extern CredNetConstants credNetConstants;

mutex lock_rates;
mutex lock_cout;

void singleSimulation(int numNodes, int cap, int amt, double threshold, int numIR, 
	int window_size, const int numTest, int burn, int mechanismGenMode)
{
	std::vector<string> v;
	// config the network
	CreditNet creditNet(numNodes);
	creditNet.genTest0Graph(threshold,numIR,cap);
	//creditNet.print();

	//creditNet.setRoutePreference(mechanismGenMode, v);
	
	// main loop
	// first window_size runs
	int failRate1 = 0;
	int failRate2 = 0;
	int failRateTotal = 0;
	int test = 0;
	vector<int> array;
	for (int i = 0; i < window_size; ++i){
		int temp;
		temp = creditNet.genInterBankTrans(amt);
		array.push_back(temp);
		failRate1 += temp;
		failRateTotal += temp;
	}
	
	for (int i = 0; i < window_size; ++i){
		int temp;
		temp = creditNet.genInterBankTrans(amt);
		array.push_back(temp);
		failRate2 += temp;
		failRateTotal += temp;
	}
	
	int cnt = 0;
	while (1){
		if (abs(failRate1 - failRate2) <= window_size * 2.0 / 1000.0){
			break;
		}
		// move on
		int temp;
		
		temp = creditNet.genInterBankTrans(amt);
		
		failRate1 = failRate1 - array[0] + array[window_size];
		failRate2 = failRate2 - array[window_size] + temp;
		failRateTotal += temp;
		array.erase(array.begin());
		array.push_back(temp);
		cnt++;
		// cout>>cnt>>endl;
	}
	double resultRate;
	lock_rates.lock();
	resultRate = failRateTotal / (cnt + 2.0 * window_size);
	// double transRate = (double) test/((double) numTest * (double) finNum * ((double) finNum-1));
	lock_rates.unlock();
	lock_cout.lock();
	cout << "threshold   " << threshold << "   SS fail rate   " << resultRate <<"   denom   "<< cnt + 2.0 * window_size <<"   num   "<<failRateTotal<<endl;

	lock_cout.unlock();
}

int main(int argc, char* argv[]){
			
	int numNodes = 200;
	double threshold;
	int mechanismGenMode = 1;
	int numIR = atoi(argv[2]);
	int amt = atoi(argv[1]);
	int cap = atoi(argv[3]);

	for (int i = 1; i <= numIR; ++i){
		credNetConstants.addIr(0.01 * i);
	}

	int window_size = 4500;
	const int iter = 8;
	int numTest = 10;
	int burn = 150;
	const int numDeg = 10;
	// double degrees [numDeg] = {0.01,0.02,0.04,0.06,0.09,0.12,0.15,
	// double degrees [numDeg] = {0.10,0.15,0.20, 0.25, 0.30};
	double degrees[numDeg] = {0.025, 0.035, 0.05, 0.075, 0.085, 0.1, 0.115, 0.125, 0.15, 0.175};
	// double degrees[numDeg] = {0.025};
	
	// 10 rounds
	for (int i = 0; i < numDeg; ++i){
		threshold = degrees[i];
		// double* rates = new double [iter];
		vector<std::thread*> threadPool;
		// double rateFinal = 0;
		
		// smooth the result
	
		for (int j = 0; j < iter; ++j){
			std::thread* singleRoundThread = new std::thread(singleSimulation,
				numNodes, cap, amt, threshold, numIR, 
				window_size, numTest, burn, mechanismGenMode
				);
			threadPool.push_back(singleRoundThread);
		}
		
		// wait for all threads to finish
		for (int j = 0; j < iter; ++j){
			threadPool[j]->join();
			delete threadPool[j];
		}

	}

	return 0;
}