#include "CN_WidgetGraph.h"

#include <iostream>
#include <unordered_map>

using namespace std;

WidgetGraph::WidgetGraph(){}

WidgetGraph::~WidgetGraph(){
	for (int i = 0; i < widgetNodes.size(); ++i){
		delete widgetNodes[i];
	}
}

template <class W>
static void addIntWidgetPair(int targetId, 
	W widgetNode, unordered_map<int, W>& map){
	std::pair<int, W> tempPair;
	tempPair.first = targetId;
	tempPair.second = widgetNode;
	map.insert(tempPair);
}

static void addWidgetNode(int & globalId, Node* node, WidgetGraph* g){
	for (auto edgePair : node->edge_in){

		for (int i = 0; i < edgePair.second->singleCreditEdges.size(); ++i){


			SingleCreditEdge* s = edgePair.second->singleCreditEdges[i];

			if (s->credit_remain->capacity != 0){
				// create widget node from credit in edge
				int id1 = globalId++;
				WidgetNode* widgetNode1 = new WidgetNode(CREDIT_IN_NODE, node, id1, edgePair.second->nodeFrom->getNodeId());
				g->widgetNodes.push_back(widgetNode1);
				addIntWidgetPair(edgePair.second->unitEdges[i].unitEdgeId, widgetNode1, node->credit_in_widget_nodes);
			}
			for (int j = 0; j < s->debt_current; ++j){
				if (0 != edgePair.second->unitEdges[i].d_current){
					// create widget node from debt out edge
					int id2 = globalId++;
					WidgetNode* widgetNode2 = new WidgetNode(DEBT_OUT_NODE, node, id2, edgePair.second->nodeFrom->getNodeId());
					g->widgetNodes.push_back(widgetNode2);
					addIntWidgetPair(edgePair.second->unitEdges[i].unitEdgeId, widgetNode2, node->debt_out_widget_nodes);
				}
			}
			
		}

	}

	for (auto edgePair : node->edge_out){

		for (int i = 0; i < edgePair.second->singleCreditEdges.size(); ++i){

			SingleCreditEdge* s = edgePair.second->singleCreditEdges[i];

			if (s->credit_remain->capacity != 0){
				// create widget node from credit in edge
				int id1 = globalId++;
				WidgetNode* widgetNode1 = new WidgetNode(CREDIT_OUT_NODE, node, id1, edgePair.second->nodeFrom->getNodeId());
				g->widgetNodes.push_back(widgetNode1);
				addIntWidgetPair(edgePair.second->unitEdges[i].unitEdgeId, widgetNode1, node->credit_in_widget_nodes);
			}
			for (int j = 0; j < s->debt_current; ++j){
				if (0 != edgePair.second->unitEdges[i].d_current){
					// create widget node from debt out edge
					int id2 = globalId++;
					WidgetNode* widgetNode2 = new WidgetNode(DEBT_IN_NODE, node, id2, edgePair.second->nodeFrom->getNodeId());
					g->widgetNodes.push_back(widgetNode2);
					addIntWidgetPair(edgePair.second->unitEdges[i].unitEdgeId, widgetNode2, node->debt_out_widget_nodes);
				}
			}
		}

	}
}

void WidgetGraph::constructWidget(Graph* graphT){

	// construct all nodes
	int globalId = 0;
	for (auto nodePair : graphT->nodes){
		Node* node = nodePair.second;
		addWidgetNode(globalId, node, this);
	}

	// add all outer edges between widget nodes
	for (auto nodePair : graphT->nodes){

		Node* node = nodePair.second;
		
		for (auto edgePair : node->edge_in){
			Node* target = graphT->nodes.find(edgePair.second->nodeFrom->getNodeId())->second;
			if (edgePair.second->get_d_current() == 0){
				WidgetNode* widgetNode1 = node->credit_in_widget_nodes.find(target->getNodeId())->second;
				WidgetNode* widgetNode2 = target->credit_out_widget_nodes.find(node->getNodeId())->second;
				this->addEdge(widgetNode1, widgetNode2, 
					edgePair.second->get_c_remain(), edgePair.second->get_interest_rate(), 0, CREDIT_WIDGET_EDGE);
			} else {
				WidgetNode* widgetNode1 = node->debt_out_widget_nodes.find(target->getNodeId())->second;
				WidgetNode* widgetNode2 = target->debt_in_widget_nodes.find(node->getNodeId())->second;
				this->addEdge(widgetNode2, widgetNode1, 
					edgePair.second->get_d_current(), edgePair.second->get_interest_rate(), 0, DEBT_WIDGET_EDGE);
			}

		}
	}

	// inner widget edges
	for (auto nodePair : graphT->nodes){

		Node* node = nodePair.second;

		for (auto inwardFlowWidget : node->credit_out_widget_nodes){
			for (auto outwardFlowWidget : node->credit_in_widget_nodes){
				addEdge(inwardFlowWidget.second, outwardFlowWidget.second, 1, 2.0, 
					node->edge_out.find(inwardFlowWidget.second->targetNodeId)->second->get_interest_rate() 
					- node->edge_in.find(outwardFlowWidget.second->targetNodeId)->second->get_interest_rate(), 
					INNER_WIDGET_EDGE
				);				
			}
			for (auto outwardFlowWidget : node->debt_in_widget_nodes){
				addEdge(inwardFlowWidget.second, outwardFlowWidget.second, 1, 2.0, 
					node->edge_out.find(inwardFlowWidget.second->targetNodeId)->second->get_interest_rate() 
					- node->edge_out.find(outwardFlowWidget.second->targetNodeId)->second->get_interest_rate(),
					INNER_WIDGET_EDGE
				);
			}
		}
		for (auto inwardFlowWidget : node->debt_out_widget_nodes){
			for (auto outwardFlowWidget : node->credit_in_widget_nodes){
				addEdge(inwardFlowWidget.second, outwardFlowWidget.second, 1, 2.0, 
					node->edge_in.find(inwardFlowWidget.second->targetNodeId)->second->get_interest_rate() 
					- node->edge_in.find(outwardFlowWidget.second->targetNodeId)->second->get_interest_rate(),
					INNER_WIDGET_EDGE
				);				
			}
			for (auto outwardFlowWidget : node->debt_in_widget_nodes){
				addEdge(inwardFlowWidget.second, outwardFlowWidget.second, 1, 2.0, 
					node->edge_in.find(inwardFlowWidget.second->targetNodeId)->second->get_interest_rate() 
					- node->edge_out.find(outwardFlowWidget.second->targetNodeId)->second->get_interest_rate(),
					INNER_WIDGET_EDGE
				);
			}
		}
	}


	// super source, super destination
	// WidgetNode* source = new WidgetNode(SUPER_SOURCE, this->src, globalId++, -1);
	// WidgetNode* dest = new WidgetNode(SUPER_DEST, this->dest, globalId++, -1);
	// this->widgetNodes.push_back(source);
	// this->widgetNodes.push_back(dest);
	// for (auto it : this->src->credit_in_widget_nodes){
	// 	this->addEdge(source, it.second, 1, 2.0, 0);
	// }
	// for (auto it : this->src->debt_in_widget_nodes){
	// 	this->addEdge(source, it.second, 1, 2.0, 0);
	// }
	// for (auto it : this->dest->credit_out_widget_nodes){
	// 	this->addEdge(dest, it.second, 1, 2.0, 0);
	// }
	// for (auto it : this->dest->debt_out_widget_nodes){
	// 	this->addEdge(dest, it.second, 1, 2.0, 0);
	// }

}

// from node1 to node2, capability of routing
void WidgetGraph::addEdge(WidgetNode* node1, WidgetNode* node2, 
	int capacity, double ir, double ir_diff, WidgetEdgeType type){

	WidgetEdge* edge = new WidgetEdge(ir, ir_diff, capacity, node1, node2, type);
	addIntWidgetPair(node2->getGlobalId(), edge, node1->edge_out);
	addIntWidgetPair(node1->getGlobalId(), edge, node2->edge_in);

}



void WidgetGraph::setupSrcAndDest(Node* srcT, Node* destT, double paymentT){
	src = srcT;
	dest = destT;
	payment = paymentT;
}





void WidgetGraph::constructWidget(Graph* graphT){
	// widget nodes
	int globalId = 0;
	for (auto atomicEdgePair : graphT->atomicEdges){

		WidgetNode* w1 = new WidgetNode(atomicEdgePair.second->originEdge->nodeFrom, globalId, atomicEdgePair.first);
		WidgetNode* w2 = new WidgetNode(atomicEdgePair.second->originEdge->nodeTo, globalId, atomicEdgePair.first);
		widgetNodes.push_back(w1);
		widgetNodes.push_back(w2);
		// mark on atomic edge
		atomicEdgePair.second->fromWidget = w1;
		atomicEdgePair.second->toWidget = w2;

	}

	// outer edges
	for (auto atomicEdgePair : graphT->atomicEdges){
		AtomicEdge* temp = atomicEdgePair.second;
		if (temp->isDebt){
			this->addEdge(temp->fromWidget, temp->toWidget, temp->interest_rate, 0);
		} else {
			this->addEdge(temp->toWidget, temp->fromWidget, temp->interest_rate, 0);
		}
		
	}

	// inner edges
	

}


// from node1 to node2, capability of routing
void WidgetGraph::addEdge(WidgetNode* node1, WidgetNode* node2, 
	int capacity, double ir, double ir_diff){

	WidgetEdge* edge = new WidgetEdge(ir, ir_diff, capacity, node1, node2, type);
	node1->edge_out[node2->getGlobalId()] = edge;
	node2->edge_in[node1->getGlobalId()] = edge;

}

