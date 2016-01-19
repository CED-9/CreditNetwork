
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

mutex lock_rates;
mutex lock_cout;

void singleSimulation(int numNodes, int cap, int amt, double threshold, int numIR, 
	int window_size, const int numTest, int burn, int mechanismGenMode, double* resultRate)
{
	std::vector<string> v;
	// config the network
	CreditNet creditNet;
	creditNet.genTest0Graph(threshold,numIR,numNodes,cap);
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
	lock_rates.lock();
	*resultRate = failRateTotal / (cnt + 2.0 * window_size + 1.0);
	// double transRate = (double) test/((double) numTest * (double) finNum * ((double) finNum-1));
	lock_rates.unlock();
	lock_cout.lock();
	cout << "threshold   " << threshold << "   SS fail rate   " << *resultRate <<"   denom   "<< cnt + 2.0 * window_size + 1.0<<"   num   "<<failRateTotal<<endl;

	lock_cout.unlock();
}

int main(int argc, char* argv[]){
	// CreditNet creditNet;
	// creditNet.genTest0Graph(1,1,100,1);
	// int temp;
	// temp =  creditNet.genInterBankTrans(1);




	// 		double* rates = new double [1];
	// 	vector<std::thread*> threadPool;
	// std::thread* singleRoundThread = new std::thread(singleSimulation,
	// 			1, 1, 1, 1, 1, 
	// 			1, 1, 1, 1,
	// 			rates);
	// 		threadPool.push_back(singleRoundThread);
			
	int numNodes = 200;
	double threshold;
	int mechanismGenMode = 1;
	int numIR = atoi(argv[2]);
	int amt = atoi(argv[1]);
	int cap = atoi(argv[3]);
	int window_size = 100;
	const int iter = 10;
	int numTest = 10;
	int burn = 150;
	const int numDeg = 1;
	// double degrees [numDeg] = {0.01,0.02,0.04,0.06,0.09,0.12,0.15,
	// double degrees [numDeg] = {0.10,0.15,0.20, 0.25, 0.30};
	//double degrees[numDeg] = {0.025, 0.035, 0.05, 0.075, 0.085, 0.1, 0.115, 0.125, 0.15, 0.175};
	double degrees[numDeg] = {1.1};

	for (int i = 0; i < numDeg; ++i){
		threshold = degrees[i];
			
		for (int j = 0; j < iter; ++j){
			
			std::vector<string> v;
			// config the network
			CreditNet creditNet;
			creditNet.genTest0Graph(threshold,numIR,numNodes,cap);
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
			cout << "threshold   " << threshold << "   SS fail rate   " << failRateTotal <<"   denom   "<< cnt + 2.0 * window_size + 1.0<<"   num   "<<failRateTotal<<endl;
		}
		
		// wait for all threads to finish
	
	}
	/*
	// 10 rounds
	for (int i = 0; i < numDeg; ++i){
		threshold = degrees[i];
		double* rates = new double [iter];
		vector<std::thread*> threadPool;
		// double rateFinal = 0;
		
		// smooth the result
	
		for (int j = 0; j < iter; ++j){
			std::thread* singleRoundThread = new std::thread(singleSimulation,
				numNodes, cap, amt, threshold, numIR, 
				window_size, numTest, burn, mechanismGenMode,
				rates + j);
			threadPool.push_back(singleRoundThread);
		}
		
		// wait for all threads to finish
		for (int j = 0; j < iter; ++j){
			threadPool[j]->join();
			delete threadPool[j];
		}

		// double rate_avg = 0;
		// for (int j = 0; j < iter; ++j){
		// 	rate_avg += rates[j];
		// }
		// cout << "Threshold   " << threshold << "   SS Fail rate   " << rate_avg/iter << endl;
		delete [] rates;
	}
	*/
	// Graph graph;
	// graph.generateTestGraph2();
	// Graph graph(100);
	// graph.genTest0Graph(0.06, 5);
	// graph.print();
	
	// Graph graph;
	// graph.generateTestGraph3();
	// graph.print();

	// CplexConverter converter;
	// converter.constructCplex(&graph, graph.nodes[0], graph.nodes[3], 1);
	// converter.printInput();
	
	// LpSolver lpSolver;
	// lpSolver.buildLpProblem(converter);
	// // converter.printResult();
	// converter.copyBack();

	// graph.print();	


	// CreditNet creditNet;
	// creditNet.genTest0Graph(1,3,);
	// creditNet.genInterBankTrans(1);
	// creditNet.print();

	// int cnt = 0;
	// for (int i = 0; i < 100; ++i){
	// 	vector<Edge*> path;
	// 	Node* src = graph.nodes.find(rand()%100)->second;
	// 	Node* dest = graph.nodes.find(rand()%100)->second;
	// 	bool result = Searcher::bfsIRConstraint(0.03, &graph, src, dest, path);

	// 	cout << "result: " << result << endl;
	// 	if (result){
	// 		cnt++;
	// 	}

	// 	Executer::execute(path, src, dest);
	// }
	// cout << cnt << endl;

	// Node* src = graph.nodes.find(0)->second;
	// Node* dest = graph.nodes.find(5)->second;
	// vector<Edge*> path;
	// bool result = Searcher::bfsIRConstraint(0.3, &graph, src, dest, path);

	// cout << "result: " << result << endl;

	// for (int i = 0; i < path.size(); ++i){
	// 	cout << path[i]->nodeFrom->getNodeId() 
	//  		<< " " << path[i]->nodeTo->getNodeId() << " "
	//  		<< path[i]->get_interest_rate() << ", ";
	// }
	// cout << endl;

	// Executer::execute(path, src, dest);
	// graph.print();

	// result = Searcher::bfsIRConstraint(0.5, &graph, dest, src, path);
	// cout << "result: " << result << endl;
	// for (int i = 0; i < path.size(); ++i){
	// 	cout << path[i]->nodeFrom->getNodeId() 
	//  		<< " " << path[i]->nodeTo->getNodeId() << " "
	//  		<< path[i]->get_interest_rate() << ", ";
	// }
	// cout << endl;
	// Executer::execute(path, src, dest);
	// graph.print();

	// for (auto it : graph.nodes){
	// 	cout << it.second->getNodeId() << it.second->totalIR / it.second->transactionNum << endl;
	// }


	// CreditNet creditNet(100);
	// creditNet.genTest0Graph(0.20, 1);
	// creditNet.print();

	return 0;
}