
#include "CN_Graph.h"
#include "CN_Constants.h"
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

extern CredNetConstants credNetConstants;

/////////////////////////////////////////////////////////////////////////
/* Graph basics */
/////////////////////////////////////////////////////////////////////////
Graph::Graph(){
	this->atomicGlobalId = 0;
}

Graph::Graph(int nodeNumT){
	this->nodeNum = nodeNumT;
	this->atomicGlobalId = 0;
	for (int i = 0; i < nodeNum; ++i){
		Node* temp = new Node(i);
		std::pair<int, Node*> tempPair;
		tempPair.first = i;
		tempPair.second = temp;
		nodes.insert(tempPair);
	}
}

Graph::~Graph(){
	for (auto& it : nodes){
		delete it.second;
	}
	for (auto& it : edges){
		delete it;
	}
}



/////////////////////////////////////////////////////////////////////////
/* Generate Initial Network */
/////////////////////////////////////////////////////////////////////////
// void Graph::genTest0Graph(double threshold, int numIR, int cap){

// 	for (int i = 0; i < nodeNum; ++i){
// 		Node* temp = new Node(i);
// 		std::pair<int, Node*> tempPair;
// 		tempPair.first = i;
// 		tempPair.second = temp;
// 		nodes.insert(tempPair);
// 	}

// 	// default_random_engine generator;
// 	// uniform_real_distribution<double> distribution(0.0, 1.0);
// 	for (int i = 0; i < nodeNum; i++){
// 		for (int j = i+1; j < nodeNum; j++){

// 			double num = credNetConstants.uniformDoubleDistribution(
// 				credNetConstants.gloabalGenerator);

// 			double ir = (credNetConstants.uniformIntDistribution(
// 				credNetConstants.gloabalGenerator) % numIR + 1);
			
// 			if (num < threshold){
// 				if (rand()%2 == 1) {
// 					// this->addUnitEdge(nodes.find(i)->second, nodes.find(j)->second, ir, rand()%2);
// 					this->addMultiEdge(nodes.find(i)->second, nodes.find(j)->second, ir, 0.0, 0, cap);
// 				} else {
// 					// this->addUnitEdge(nodes.find(j)->second, nodes.find(i)->second, ir, rand()%2);
// 					this->addMultiEdge(nodes.find(j)->second, nodes.find(i)->second, ir, 0.0, 0, cap);
// 				}
// 			}

// 		}
// 	}


// }

// void Graph::genTest1Graph(double threshold, int numIR, int cap){

// 	for (int i = 0; i < nodeNum; ++i){
// 		Node* temp = new Node(i);
// 		std::pair<int, Node*> tempPair;
// 		tempPair.first = i;
// 		tempPair.second = temp;
// 		nodes.insert(tempPair);
// 	}

// 	// default_random_engine generator;
// 	// uniform_real_distribution<double> distribution(0.0, 1.0);
// 	for (int i = 0; i < nodeNum; i++){

// 		int count = threshold / 2 * nodeNum + 1;
// 		while(count > 0){
			
// 			double ir = (credNetConstants.uniformIntDistribution(
// 				credNetConstants.gloabalGenerator) % numIR + 1);

// 			double id = (credNetConstants.uniformIntDistribution(
// 				credNetConstants.gloabalGenerator) % nodeNum);
// 			if (id == i){
// 				continue;
// 			}
// 			this->addMultiEdge(nodes.find(id)->second, nodes.find(i)->second, ir, 0.0, 0, cap);
// 			count--;
// 		}

// 	}
// }


// void Graph::generateTestGraph2(){

// 	nodeNum = 8;
// 	for (int i = 0; i < 8; ++i){
// 		Node* temp = new Node(i);
// 		std::pair<int, Node*> tempPair;
// 		tempPair.first = i;
// 		tempPair.second = temp;
// 		nodes.insert(tempPair);
// 	}
// 	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.2, 0.4, 2, 2);

// 	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.3, 0.3, 3, 3);

// 	this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.3, 0.0, 0, 2);

// 	this->addMultiEdge(nodes.find(2)->second, nodes.find(4)->second, 0.2, 0.2, 4, 4);

// 	this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.2, 0.0, 0, 2);

// 	this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.4, 0.0, 0, 1);

// 	this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.2, 0.0, 0, 3);

// 	this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.4, 0.0, 0, 2);

// 	this->addMultiEdge(nodes.find(6)->second, nodes.find(1)->second, 0.2, 0.0, 0, 2);

// 	this->addMultiEdge(nodes.find(7)->second, nodes.find(6)->second, 0.3, 0.0, 0, 1);

// 	this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.2, 0.0, 0, 2);

// }


// void Graph::generateTestGraph3(){

// 	nodeNum = 14;
// 	for (int i = 0; i < nodeNum; ++i){
// 		nodes[i] = new Node(i);
// 	}

// 	this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.1, 0.1, 0, 10);

// 	this->addMultiEdge(nodes.find(1)->second, nodes.find(4)->second, 0.2, 0.2, 0, 10);
// 	this->addMultiEdge(nodes.find(1)->second, nodes.find(5)->second, 0.3, 0.3, 0, 10);

// 	this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.1, 0.1, 0, 10);

// 	this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.3, 0.3, 0, 10);

// 	this->addMultiEdge(nodes.find(4)->second, nodes.find(0)->second, 0.2, 0.2, 0, 10);
// 	this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.3, 0.3, 0, 10);
// 	this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.1, 0.1, 0, 10);

// 	this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.3, 0.3, 0, 10);
// 	this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.1, 0.1, 0, 10);
// 	this->addMultiEdge(nodes.find(5)->second, nodes.find(9)->second, 0.2, 0.2, 0, 10);

// 	this->addMultiEdge(nodes.find(6)->second, nodes.find(2)->second, 0.2, 0.2, 0, 10);
// 	this->addMultiEdge(nodes.find(6)->second, nodes.find(5)->second, 0.1, 0.1, 0, 10);
	
// 	this->addMultiEdge(nodes.find(7)->second, nodes.find(3)->second, 0.2, 0.2, 0, 10);

// 	this->addMultiEdge(nodes.find(8)->second, nodes.find(4)->second, 0.2, 0.2, 0, 10);
// 	this->addMultiEdge(nodes.find(8)->second, nodes.find(5)->second, 0.2, 0.2, 0, 10);
// 	this->addMultiEdge(nodes.find(8)->second, nodes.find(7)->second, 0.3, 0.3, 0, 10);
// 	this->addMultiEdge(nodes.find(8)->second, nodes.find(12)->second, 0.1, 0.1, 0, 10);

// 	this->addMultiEdge(nodes.find(9)->second, nodes.find(6)->second, 0.1, 0.1, 0, 10);
// 	this->addMultiEdge(nodes.find(9)->second, nodes.find(8)->second, 0.3, 0.3, 0, 10);
// 	this->addMultiEdge(nodes.find(9)->second, nodes.find(13)->second, 0.3, 0.3, 0, 10);

// 	this->addMultiEdge(nodes.find(10)->second, nodes.find(6)->second, 0.2, 0.2, 0, 10);
// 	this->addMultiEdge(nodes.find(10)->second, nodes.find(9)->second, 0.3, 0.3, 0, 10);

// 	this->addMultiEdge(nodes.find(11)->second, nodes.find(7)->second, 0.2, 0.2, 0, 10);
// 	this->addMultiEdge(nodes.find(11)->second, nodes.find(8)->second, 0.2, 0.2, 0, 10);

// 	this->addMultiEdge(nodes.find(12)->second, nodes.find(9)->second, 0.3, 0.3, 0, 10);
// 	this->addMultiEdge(nodes.find(12)->second, nodes.find(11)->second, 0.2, 0.2, 0, 10);

// 	this->addMultiEdge(nodes.find(13)->second, nodes.find(10)->second, 0.2, 0.2, 10, 10);
// 	this->addMultiEdge(nodes.find(13)->second, nodes.find(12)->second, 0.1, 0.1, 10, 10);

// 	// this->addMultiEdge(nodes.find(0)->second, nodes.find(1)->second, 0.1, 0.1, 10, 10);

// 	// this->addMultiEdge(nodes.find(1)->second, nodes.find(4)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(1)->second, nodes.find(5)->second, 0.3, 0.3, 10, 10);

// 	// this->addMultiEdge(nodes.find(2)->second, nodes.find(1)->second, 0.1, 0.1, 10, 10);

// 	// this->addMultiEdge(nodes.find(3)->second, nodes.find(0)->second, 0.3, 0.3, 10, 10);

// 	// this->addMultiEdge(nodes.find(4)->second, nodes.find(0)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(4)->second, nodes.find(3)->second, 0.3, 0.3, 10, 10);
// 	// this->addMultiEdge(nodes.find(4)->second, nodes.find(7)->second, 0.1, 0.1, 10, 10);

// 	// this->addMultiEdge(nodes.find(5)->second, nodes.find(2)->second, 0.3, 0.3, 10, 10);
// 	// this->addMultiEdge(nodes.find(5)->second, nodes.find(4)->second, 0.1, 0.1, 10, 10);
// 	// this->addMultiEdge(nodes.find(5)->second, nodes.find(9)->second, 0.2, 0.2, 10, 10);

// 	// this->addMultiEdge(nodes.find(6)->second, nodes.find(2)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(6)->second, nodes.find(5)->second, 0.1, 0.1, 10, 10);
	
// 	// this->addMultiEdge(nodes.find(7)->second, nodes.find(3)->second, 0.2, 0.2, 10, 10);

// 	// this->addMultiEdge(nodes.find(8)->second, nodes.find(4)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(8)->second, nodes.find(5)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(8)->second, nodes.find(7)->second, 0.3, 0.3, 10, 10);
// 	// this->addMultiEdge(nodes.find(8)->second, nodes.find(12)->second, 0.1, 0.1, 10, 10);

// 	// this->addMultiEdge(nodes.find(9)->second, nodes.find(6)->second, 0.1, 0.1, 10, 10);
// 	// this->addMultiEdge(nodes.find(9)->second, nodes.find(8)->second, 0.3, 0.3, 10, 10);
// 	// this->addMultiEdge(nodes.find(9)->second, nodes.find(13)->second, 0.3, 0.3, 10, 10);

// 	// this->addMultiEdge(nodes.find(10)->second, nodes.find(6)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(10)->second, nodes.find(9)->second, 0.3, 0.3, 10, 10);

// 	// this->addMultiEdge(nodes.find(11)->second, nodes.find(7)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(11)->second, nodes.find(8)->second, 0.2, 0.2, 10, 10);

// 	// this->addMultiEdge(nodes.find(12)->second, nodes.find(9)->second, 0.3, 0.3, 10, 10);
// 	// this->addMultiEdge(nodes.find(12)->second, nodes.find(11)->second, 0.2, 0.2, 10, 10);

// 	// this->addMultiEdge(nodes.find(13)->second, nodes.find(10)->second, 0.2, 0.2, 10, 10);
// 	// this->addMultiEdge(nodes.find(13)->second, nodes.find(12)->second, 0.1, 0.1, 10, 10);

// }
