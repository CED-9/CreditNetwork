#ifndef CN_Searcher
#define CN_Searcher

#include "CN_Node.h"

class Searcher{

public:

    // bfs, credit line first, debt after
    std::list <Node*> pathMixed;
	double pathCapacityMixed();
	bool bfsMixed(Node* node1, Node* node2);
	Status pathFillMixed(double cap);
	double maxFlowMixed(Node* node1, Node* node2);

};


double Searcher::maxFlowMixed(Node* node1, Node* node2){
	double currentFlow = 0;
	double flow = 0;
	while (bfsMixed(node1, node2)){
		currentFlow = pathCapacityMixed();
		//cout<<"curr flow: "<<currentFlow<<endl;
		if (currentFlow < 0.0001) {
			break;
		}
		flow += currentFlow;
		pathFillMixed(currentFlow);
	}
	return flow;
}

// Fill the current path, up to the input current flow
// no discretion (eg. no interest rate constraints)
// first use credit, then debt
Status Searcher::pathFillMixed(double currentFlow){
	//cout<<"pathfill: "<<currentFlow<<endl;
	Node* node1 = pathMixed.front();
	pathMixed.pop_front();
	Node* node2;
	double credit = 0;
	Status status; 
	try{
		while (!pathMixed.empty()){
			node2 = pathMixed.front();
			pathMixed.pop_front();

			credit = node1->getCreditFrom(node2, status);
			//cout<<"credit: "<<node2->getNodeID()<<" "<<credit<<endl;
			// Use credit first, then debt
			if (currentFlow <= credit){
				status = node1->setDebtTo(node2, currentFlow, ADD);
				if (status != GOOD){ throw status; }
				status = node2->setDebtFrom(node1, currentFlow, ADD);
				if (status != GOOD){ throw status; }
			}
			else {
				status = node1->setDebtTo(node2, credit, ADD);
				if (status != GOOD){ throw status; }
				status = node2->setDebtFrom(node1, credit, ADD);
				if (status != GOOD){ throw status; }
				
				//cout<<"curr - cre: "<<currentFlow <<" "<< credit<<endl;
				status = node1->setDebtFrom(node2, currentFlow - credit, SUB);
				if (status != GOOD){
					//cout<<"ccccc"<<endl;
					throw status;
				}
				status = node2->setDebtTo(node1, currentFlow - credit, SUB);
				if (status != GOOD){ throw status; }
			}
			node1 = node2;
		}
	}
	catch (Status error){
		return error; 
	}
	return GOOD; 
}

// Get the max flow for current path
// max flow = credit + debt IOUs
// no interest rate constraint, no discretion
double Searcher::pathCapacityMixed(){
	std::list <Node*> pathTemp = pathMixed;
	Status status; 
	Node* node1 = pathMixed.front();
	pathMixed.pop_front();
	Node* node2;

	double cap = (double)(2 * CREDIT_MAX_FIN_FIN);

	while (!pathMixed.empty()){
		node2 = pathMixed.front();
		pathMixed.pop_front();
		cap = min(cap,
			node1->getDebtFrom(node2, status)
			+ node1->getCreditFrom(node2, status));
		node1 = node2;
	}

	while (!pathTemp.empty()){
		pathMixed.push_back(pathTemp.front());
		pathTemp.pop_front();
	}
	return cap;
}

// bfs, no discretion
// path exists when there is credit or debt relation
// input: two node*
// return: true if path (capacity > 0) exists
bool Searcher::bfsMixed(Node* node1, Node* node2){
	this->pathMixed.clear(); 
	Status status; 
	queue <Node*> tempQueue; 
	set <Node*> tempVisited; 
	Node* front = NULL;
	Node* prev[NODE_MAX_TOTAL];
	tempVisited.insert(node1); 
	tempQueue.push(node1); 	
	for(int i=0; i<NODE_MAX_TOTAL; i++){
		prev[i] = NULL;
	}
	while(tempQueue.size() != 0){
		front = tempQueue.front(); 
		tempQueue.pop(); 
		if(node2 == front){
			break; 
		}

		// someone issued c 
		for(int i=0; i < front->edge_in.size(); i++){ 
			if(tempVisited.end() != tempVisited.find(front->edge_in[i].nodeFrom)
				|| front->getCreditFrom(front->edge_in[i].nodeFrom, status) <= 0.000001)
			{ 
				continue; 
			}
			prev[front->edge_in[i].nodeFrom->getNodeID()] = front; 
			tempVisited.insert(front->edge_in[i].nodeFrom); 
			tempQueue.push(front->edge_in[i].nodeFrom); 
		}
		// someone owed d, but issued no c
		for(int i=0; i < front->edge_out.size(); i++){
			if(tempVisited.end() != tempVisited.find(front->edge_out[i].nodeTo)
				|| front->getDebtFrom(front->edge_out[i].nodeTo, status) <= 0.000001)
			{ 
				continue; 
			}
			prev[front->edge_out[i].nodeTo->getNodeID()] = front; 
			tempVisited.insert(front->edge_out[i].nodeTo); 
			tempQueue.push(front->edge_out[i].nodeTo); 
		}
	}
	if(front != node2){
		//cout<<"No path exists! "<<endl;
		this->pathMixed.clear(); 
		return false; 
	}
	while(front){
		this->pathMixed.push_front(front); 
		front = prev[front->getNodeID()];
	}
	return true; 
}







#endif