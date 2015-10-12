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

void WidgetGraph::constructWidget(Graph* graphT){

	// construct all nodes
	int globalId = 0;
	for (auto nodePair : graphT->nodes){

		Node* node = nodePair.second;

		for (auto edgePair : node->edge_in){

			if (edgePair.second->get_d_out_current() == 0){

				int id1 = globalId++;
				WidgetNode* widgetNode1 = new WidgetNode(CREDIT_IN_NODE, node, id1, edgePair.first);
				this->widgetNodes.push_back(widgetNode1);
				addIntWidgetPair(edgePair.first, widgetNode1, node->credit_in_widget_nodes);

			} else {

				int id2 = globalId++;
				WidgetNode* widgetNode2 = new WidgetNode(DEBT_OUT_NODE, node, id2, edgePair.first);
				this->widgetNodes.push_back(widgetNode2);
				addIntWidgetPair(edgePair.first, widgetNode2, node->debt_out_widget_nodes);

			}
			
			// int id1 = globalId++;
			// int id2 = globalId++;
			
			// WidgetNode* widgetNode1 = new WidgetNode(CREDIT_IN_NODE, node, id1, edgePair.first);
			// WidgetNode* widgetNode2 = new WidgetNode(DEBT_OUT_NODE, node, id2, edgePair.first);
			
			// addIntWidgetPair(id1, widgetNode1, this->widgetNodes);
			// addIntWidgetPair(id2, widgetNode2, this->widgetNodes);

			// addIntWidgetPair(edgePair.first, widgetNode1, node->credit_in_widget_nodes);
			// addIntWidgetPair(edgePair.first, widgetNode2, node->debt_out_widget_nodes);

		}

		for (auto edgePair : node->edge_out){

			if (edgePair.second->get_d_in_current() == 0){
				int id1 = globalId++;
				WidgetNode* widgetNode1 = new WidgetNode(CREDIT_OUT_NODE, node, id1, edgePair.first);
				this->widgetNodes.push_back(widgetNode1);
				addIntWidgetPair(edgePair.first, widgetNode1, node->credit_out_widget_nodes);
			
			} else {
				int id2 = globalId++;
				WidgetNode* widgetNode2 = new WidgetNode(DEBT_IN_NODE, node, id2, edgePair.first);
				this->widgetNodes.push_back(widgetNode2);
				addIntWidgetPair(edgePair.first, widgetNode2, node->debt_in_widget_nodes);
			}

			// int id1 = globalId++;
			// int id2 = globalId++;
			
			// WidgetNode* widgetNode1 = new WidgetNode(CREDIT_OUT_NODE, node, id1, edgePair.first);
			// WidgetNode* widgetNode2 = new WidgetNode(DEBT_IN_NODE, node, id2, edgePair.first);
			
			// addIntWidgetPair(id1, widgetNode1, this->widgetNodes);
			// addIntWidgetPair(id2, widgetNode2, this->widgetNodes);

			// addIntWidgetPair(edgePair.first, widgetNode1, node->credit_out_widget_nodes);
			// addIntWidgetPair(edgePair.first, widgetNode2, node->debt_in_widget_nodes);
		}
	}

	// add all outer edges between widget nodes
	for (auto nodePair : graphT->nodes){

		Node* node = nodePair.second;
		
		for (auto edgePair : node->edge_in){
			Node* target = graphT->nodes.find(edgePair.second->getNodeFromId())->second;
			if (edgePair.second->get_d_out_current() == 0){
				WidgetNode* widgetNode1 = node->credit_in_widget_nodes.find(target->getNodeId())->second;
				WidgetNode* widgetNode2 = target->credit_out_widget_nodes.find(node->getNodeId())->second;
				this->addEdge(widgetNode1, widgetNode2, 
					edgePair.second->get_c_in_remain(), edgePair.second->get_interest_rate(), 0, CREDIT_WIDGET_EDGE);
			} else {
				WidgetNode* widgetNode1 = node->debt_out_widget_nodes.find(target->getNodeId())->second;
				WidgetNode* widgetNode2 = target->debt_in_widget_nodes.find(node->getNodeId())->second;
				this->addEdge(widgetNode2, widgetNode1, 
					edgePair.second->get_d_out_current(), edgePair.second->get_interest_rate(), 0, DEBT_WIDGET_EDGE);
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







