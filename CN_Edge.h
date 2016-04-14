#ifndef CN_Edge
#define CN_Edge


#include "CN_Node.h"
#include "CN_AtomicEdge.h"
#include "CN_SingleCreditEdge.h"

#include <vector>


class Edge{

public:

	Node* nodeFrom;
	Node* nodeTo;
	vector <SingleCreditEdge*> singleCreditEdges;

	Edge(Node* nodeFromT, Node* nodeToT){
		this->nodeFrom = nodeFromT;
		this->nodeTo = nodeToT;
	}

	~Edge(){
		for (int i = 0; i < singleCreditEdges.size(); ++i){
			delete singleCreditEdges[i];
		}
	}

};


#endif