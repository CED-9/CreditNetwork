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


void WidgetGraph::setupSrcAndDest(Node* srcT, Node* destT, double paymentT){
	src = srcT;
	dest = destT;
	payment = paymentT;
}

// from node1 to node2, capability of routing
void WidgetGraph::addEdge(WidgetNode* node1, WidgetNode* node2, 
	int capacity, double ir, double ir_diff, WidgetEdgeType type, AtomicEdge* a){

	WidgetEdge* edge = new WidgetEdge(ir, ir_diff, capacity, node1, node2, type, a);
	node1->edge_out[node2->getGlobalId()] = edge;
	node2->edge_in[node1->getGlobalId()] = edge;

}


static void helpAddInnerWidgetEdge(WidgetGraph*g, WidgetNode* from, WidgetNode* to){

	if (from->originAtomicEdge->interest_rate < to->originAtomicEdge->interest_rate){
		return;
	}
	g->addEdge(from, to, from->originAtomicEdge->capacity + 
		to->originAtomicEdge->capacity, -1, 
		from->originAtomicEdge->interest_rate - 
		to->originAtomicEdge->interest_rate, 
		INNER_WIDGET_EDGE, from->originAtomicEdge);
}

void WidgetGraph::constructWidget(Graph* graphT){
	// widget nodes
	int globalId = 0;
	for (auto atomicEdgePair : graphT->atomicEdges){
		
		AtomicEdge* atomicEdge = atomicEdgePair.second;
		WidgetNode* w1 = NULL;
		WidgetNode* w2 = NULL;
		Node* from = atomicEdge->originEdge->nodeFrom;
		Node* to = atomicEdge->originEdge->nodeTo;

		if (atomicEdge->isDebt) {
			w1 = new WidgetNode(DEBT_IN_NODE, from, globalId++, to->getNodeId(), atomicEdge);
			w2 = new WidgetNode(DEBT_OUT_NODE, to, globalId++, from->getNodeId(), atomicEdge);
			from->debt_in_widget_nodes[atomicEdge->atomicEdgeId] = w1;
			to->debt_out_widget_nodes[atomicEdge->atomicEdgeId] = w2;
		} else {
			w1 = new WidgetNode(CREDIT_OUT_NODE, from, globalId++, to->getNodeId(), atomicEdge);
			w2 = new WidgetNode(CREDIT_IN_NODE, to, globalId++, from->getNodeId(), atomicEdge);
			from->credit_out_widget_nodes[atomicEdge->atomicEdgeId] = w1;
			to->credit_in_widget_nodes[atomicEdge->atomicEdgeId] = w2;
		}

		widgetNodes.push_back(w1);
		widgetNodes.push_back(w2);
		// mark on atomic edge
		atomicEdgePair.second->fromWidget = w1;
		atomicEdgePair.second->toWidget = w2;

	}

	// outer widget edges
	for (auto atomicEdgePair : graphT->atomicEdges){
		AtomicEdge* temp = atomicEdgePair.second;
		if (temp->isDebt){
			// debt, flow: from->to
			this->addEdge(temp->fromWidget, temp->toWidget, 
				temp->capacity, temp->interest_rate, 0, DEBT_WIDGET_EDGE, atomicEdgePair.second);
		} else {
			// credit, flow: to->from
			this->addEdge(temp->toWidget, temp->fromWidget, 
				temp->capacity, temp->interest_rate, 0, CREDIT_WIDGET_EDGE, atomicEdgePair.second);
		}
	}

	// inner widget edges
	for (auto nodePair : graphT->nodes){
		Node* node = nodePair.second;

		for (auto inwardFlowWidget : node->credit_out_widget_nodes){
			for (auto outwardFlowWidget : node->credit_in_widget_nodes){
				helpAddInnerWidgetEdge(this, inwardFlowWidget.second, outwardFlowWidget.second);
			}
			for (auto outwardFlowWidget : node->debt_in_widget_nodes){
				helpAddInnerWidgetEdge(this, inwardFlowWidget.second, outwardFlowWidget.second);
			}
		}
		for (auto inwardFlowWidget : node->debt_out_widget_nodes){
			for (auto outwardFlowWidget : node->credit_in_widget_nodes){
				helpAddInnerWidgetEdge(this, inwardFlowWidget.second, outwardFlowWidget.second);
			}
			for (auto outwardFlowWidget : node->debt_in_widget_nodes){
				helpAddInnerWidgetEdge(this, inwardFlowWidget.second, outwardFlowWidget.second);
			}
		}
	}

	

}

