
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_CreditNet.h"
#include "CN_Solver.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

int main(int argc, char* argv[]){

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


	CreditNet creditNet;
	creditNet.generateTestGraph3();
	creditNet.genInterBankTrans(1);
	creditNet.print();

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