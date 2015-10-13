#include "CN_Searcher.h"

#define NODE_NUM_MAX 1000

#include <set>
#include <queue>
#include <list>
#include <unordered_map>
#include <iostream>

using namespace std;


static bool dfsHelper(Graph* graph, Node* front, Node* dest, vector<Node*>& path){

}

bool Searcher::dfs(Graph* graph, Node* src, Node* dest, vector<Node*>& path){

	path.clear();
	queue <Node*> tempQueue;
	set <Node*> tempVisited;
	Node* front = NULL;

	for (auto temp : src->edge_in){
		if (temp.second->get_d_out_current() == 0){
			tempVisited.insert(temp.second);
			tempQueue.push(temp.second);
		}
	}
	for (auto temp : src->edge_out){
		if (temp.second->get_d_out_current() == 1){
			tempVisited.insert(temp.second);
			tempQueue.push(temp.second);
		}
	}
	

	for (int i = 0; i < NODE_NUM_MAX; ++i){
		prev[i] = NULL;
	}

	while(tempQueue.size() != 0){

		front = tempQueue.front();
		tempQueue.pop();
		cout << "front: " << front->getGlobalId() << endl;
		cout << "size of edge out: " << front->edge_out.size() << endl;

		if(dest == front->originNode){
			break; 
		}

		for(auto it : front->edge_out){ 

			cout << "searching: " << it.second->nodeTo->getGlobalId() << endl;
			if(tempVisited.end() != tempVisited.find(it.second->nodeTo)){ 
				continue;
			}

			prev[it.second->nodeTo->getGlobalId()] = front; 
			tempVisited.insert(it.second->nodeTo);
			tempQueue.push(it.second->nodeTo); 
		}

	}

	if(front->originNode != dest){
		path.clear(); 
		return false; 
	}


}







bool Searcher::bfsWidget(WidgetGraph* widgetGraph, 
	Node* src, Node* dest, list<WidgetNode*>& path){

	path.clear();
	queue <WidgetNode*> tempQueue; 
	set <WidgetNode*> tempVisited; 
	WidgetNode* front = NULL;
	WidgetNode* prev[NODE_NUM_MAX];
	for (auto widget : src->credit_in_widget_nodes){
		tempVisited.insert(widget.second);
		tempQueue.push(widget.second);
	}
	for (auto widget : src->debt_in_widget_nodes){
		tempVisited.insert(widget.second);
		tempQueue.push(widget.second);
	}
	

	for (int i = 0; i < NODE_NUM_MAX; ++i){
		prev[i] = NULL;
	}

	while(tempQueue.size() != 0){

		front = tempQueue.front();
		tempQueue.pop();
		cout << "front: " << front->getGlobalId() << endl;
		cout << "size of edge out: " << front->edge_out.size() << endl;

		if(dest == front->originNode){
			break; 
		}

		for(auto it : front->edge_out){ 

			cout << "searching: " << it.second->nodeTo->getGlobalId() << endl;
			if(tempVisited.end() != tempVisited.find(it.second->nodeTo)){ 
				continue;
			}

			prev[it.second->nodeTo->getGlobalId()] = front; 
			tempVisited.insert(it.second->nodeTo);
			tempQueue.push(it.second->nodeTo); 
		}

	}

	if(front->originNode != dest){
		path.clear(); 
		return false; 
	}

	while(front){
		path.push_front(front); 
		front = prev[front->getGlobalId()];
	}

	return true;
}