// Node.C

#include "Error.h"
#include "CN_Node.h"
#include <queue>
#include <vector>
#include <fstream>

Node::Node(int id){
    this->nodeID = id;
    this->nodeType = NodeType(0);
    this->transactionNum = 0.0;
}

void Node::visualize(){
	for (int i = 0; i<edge_out.size(); i++){
		cout << this->getNodeID() << ", " << edge_out[i].nodeTo->getNodeID();
    }
}

void Node::print(){
    cout << "Node " << nodeID << endl;
    for (int i = 0; i<edge_in.size(); i++){
        cout << "in: NodeType "<< (int)edge_in[i].nodeFrom->getNodeType() << ", ID ";
        cout << edge_in[i].nodeFrom->getNodeID() << ": " << edge_in[i].d_out_current << " / "
        << edge_in[i].c_in_max << " IR: " << edge_in[i].interest_rate <<endl;
    }
    for (int i = 0; i<edge_out.size(); i++){
        cout << "out: NodeType "<< (int)edge_out[i].nodeTo->getNodeType() << ", ID ";
        cout << edge_out[i].nodeTo->getNodeID() << ": " << edge_out[i].d_in_current << " / "
        << edge_out[i].c_out_max << " IR: " << edge_out[i].interest_rate << endl;
    }
    cout << endl;
    return GOOD; 
}

