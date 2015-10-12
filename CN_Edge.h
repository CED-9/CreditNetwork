#ifndef CN_Edge
#define CN_Edge


#include <vector>
#include <iostream>

using namespace std;

struct UnitEdge{
	int c_max;
	int d_current;
	double interest_rate;
};

class Edge{

public:
	int nodeFromId;
	int nodeToId;
	vector <UnitEdge> unitEdges;

	void setEdge(int nodeFromIdT, int nodeToIdT, double interest_rate);

	~Edge();

	int get_c_remain();

	int get_d_current();

	void set_interest_rate(double interest_rateT);

	double get_interest_rate();

	void print();

};


class OutEdge : public Edge{
public:
	OutEdge(int nodeFromIdT, int nodeToIdT, double interest_rate){
		setEdge(nodeFromIdT, nodeToIdT, interest_rate);
	}

	~OutEdge(){}

	int get_c_out_remain(){
		return this->get_c_remain();
	}

	int get_d_in_current(){
		return this->get_d_current();
	}

	int getNodeToId(){
		return nodeToId;
	}

	void print(){
		cout << "To " << nodeToId;
		cout << " ... IR " << unitEdges[0].interest_rate
		<< " Credit " << unitEdges[0].c_max
		<< " Debt " << unitEdges[0].d_current;
		cout << endl;
	}

};

class InEdge : public Edge{
public:
	InEdge(int nodeFromIdT, int nodeToIdT, double interest_rate){
		setEdge(nodeFromIdT, nodeToIdT, interest_rate);
	}

	~InEdge(){}

	int get_c_in_remain(){
		return this->get_c_remain();
	}

	int get_d_out_current(){
		return this->get_d_current();
	}

	int getNodeFromId(){
		return nodeFromId;
	}

	void print(){
		cout << "From " << nodeFromId;
		cout << " ... IR " << unitEdges[0].interest_rate
		<< " Credit " << unitEdges[0].c_max
		<< " Debt " << unitEdges[0].d_current;
		cout << endl;
	}

};


#endif