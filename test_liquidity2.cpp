#include "CN_CreditNet.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;

mutex lock_rates;
mutex lock_cout;

////////////////////////////////////////////////////////////////////
void singleSimulation(
	int finNum, 
	double threshold, int numIR, 
	int window_size, const int numTest, int burn, int mechanismGenMode,
	double* resultRate)
{
	std::vector<string> v;
	// config the network
	CreditNet creditNet(finNum);
	creditNet.genTest0Graph(threshold, numIR);
	creditNet.setRoutePreference(mechanismGenMode, v);
	
	// main loop
	// first window_size runs
	int failRate1 = 0;
	int failRate2 = 0;
	int failRateTotal = 0;
	int test= 0;
	vector<int> array;
	for (int i = 0; i < window_size; ++i){
		int temp;
		temp = creditNet.genInterBankTrans();
		array.push_back(temp);
		failRate1 += temp;
		failRateTotal += temp;
	}
	
	for (int i = 0; i < window_size; ++i){
		int temp;
		temp = creditNet.genInterBankTrans();
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
		
		temp = creditNet.genInterBankTrans();
		
		failRate1 = failRate1 - array[0] + array[window_size];
		failRate2 = failRate2 - array[window_size] + temp;
		failRateTotal += temp;
		array.erase(array.begin());
		array.push_back(temp);
		cnt++;
		// cout>>cnt>>endl;
	}
	lock_rates.lock();
	*resultRate = failRateTotal / (cnt + 2.0 * window_size + 1.0);
	// double transRate = (double) test/((double) numTest * (double) finNum * ((double) finNum-1));
	lock_rates.unlock();
	lock_cout.lock();
	cout << "threshold   " << threshold << "   SS fail rate   " << *resultRate <<"   denom   "<< cnt + 2.0 * window_size + 1.0<<"   num   "<<failRateTotal<<endl;

	lock_cout.unlock();
}

// argv[1]: initialize mode
// argv[2]: number of interest rates
int main(int argc, char* argv[]){
	int finNum = 200;
	double threshold;
	int numIR = atoi(argv[2]);
	int mechanismGenMode = atoi(argv[1]);
	int window_size = 4500;
	int iter = 10;
	int numTest = 10;
	int burn = 150;
	const int numDeg = 10;
	// double degrees [numDeg] = {0.01,0.02,0.04,0.06,0.09,0.12,0.15,
	// double degrees [numDeg] = {0.10,0.15,0.20, 0.25, 0.30};
	double degrees[numDeg] = {0.05, 0.075, 0.09, 0.1, 0.125, 0.15, 0.175, 0.25, 0.3, 0.4};
	// double degrees[numDeg] = {0.09};

	// 10 rounds
	for (int i = 0; i < numDeg; ++i){
		threshold = degrees[i];
		double* rates = new double [iter];
		vector<std::thread*> threadPool;
		// double rateFinal = 0;
		
		// smooth the result

		for (int j = 0; j < iter; ++j){
			singleSimulation(finNum,
				threshold, numIR, window_size, numTest, 
				burn, mechanismGenMode, rates + j);
		}

		double rate_avg = 0;
		for (int j = 0; j < iter; ++j){
			rate_avg += rates[j];
		}
		cout << "---------------final rate: "
			<< "Threshold   " << threshold << "   SS succ rate   " << 1-rate_avg/iter << endl;
		delete [] rates;
	}
}

