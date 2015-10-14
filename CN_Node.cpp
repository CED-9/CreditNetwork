// Node.C

#include "CN_Node.h"
#include <queue>
#include <vector>
#include <fstream>

Node::Node(int id){
    this->nodeId = id;
}

Node::~Node(){
    for (auto it : edge_in){
        delete it.second;
    }
    for (auto it : edge_out){
        delete it.second;
    }
}

void Node::print(){
    cout << "Node " << this->nodeId << endl;
    for (auto it : edge_in){
        it.second->print();
    }
    for (auto it : edge_out){
        it.second->print();
    }
    cout << endl;
    return;
}


int Node::getNodeId(){ return nodeId; }