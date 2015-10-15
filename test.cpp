
#include "CN_Node.h"
#include "CN_Graph.h"
#include "CN_Searcher.h"
#include "CN_Executer.h"
#include "CN_CreditNet.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <iostream>
#include <random>

using namespace std;

int main(int argc, char* argv[]){

	// Graph graph();
	// graph.generateTestGraph();
	Graph graph(100);
	graph.genTest0Graph(0.08, 1);
	// graph.print();

	for (int i = 0; i < 100; ++i){
		vector<Edge*> path;
		Node* src = graph.nodes.find(rand()%100)->second;
		Node* dest = graph.nodes.find(rand()%100)->second;
		bool result = Searcher::bfsIRConstraint(0.5, &graph, src, dest, path);

		cout << "result: " << result << endl;

		for (int i = 0; i < path.size(); ++i){
			cout << path[i]->nodeFrom->getNodeId() 
		 		<< " " << path[i]->nodeTo->getNodeId() << " "
		 		<< path[i]->get_interest_rate() << ", ";
		}
		cout << endl;

		Executer::execute(path, src, dest);
	}

	// CreditNet creditNet(100);
	// creditNet.genTest0Graph(0.20, 1);
	// creditNet.print();

	return 0;
}