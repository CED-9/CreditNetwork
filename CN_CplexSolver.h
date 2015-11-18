#ifndef CN_CplexSolver
#define CN_CplexSolver

#include "CN_WidgetGraph.h"

class CplexConverter{

public:
	// input 
	int nnodes;
	int narcs;
	double* supply;
	int* tail;
	int* head;
	double* obj;
	double* ub;
	double* lb;

	// output
	double* x;
	double* dj;
	double* pi;
	double* slack;

	WidgetGraph* widgetNet;
	
	void constructCplex(WidgetGraph* w){
		widgetNet = w;
		nnodes = widgetNet->widgetNodes.size();
		narcs = widgetNet->widgetEdges.size();

		supply = new double [nnodes];
		tail = new int [narcs];
		head = new int [narcs];
		obj = new double [narcs];
		ub = new double [narcs];
		lb = new double [narcs];

		int cnt = 0;
		for (auto widgetEdge : widgetNet->widgetEdges){
			head[cnt] = widgetEdge->nodeFrom->getGlobalId();
			tail[cnt] = widgetEdge->nodeTo->getGlobalId();
			obj[cnt] = 0;
			ub[cnt] = widgetEdge->cap;
			lb[cnt] = 0;
			cnt++;
		}

		x = new double[narcs];
		dj = new double[narcs];
		pi = new double[nnodes];
		slack = new double[nnodes];
	}


	void printInput(){
		for (int i = 0; i < narcs; ++i){
			cout << "head: " << head[i] << " tail: " << tail[i] 
			<< " supply: " << supply[i] << " obj: " << obj[i]
			<< " ub: " << ub[i] << " lb: " << lb[i] << endl;
		}
	}
	void printResult(){
		for (int i = 0; i < narcs; ++i){
			cout << "head: " << head[i] << " tail: " << tail[i]
			<< " flow: " << x[i] << " dj: " << dj[i]
			<< " pi: " << pi[i] << " slack: " << slack[i] << endl;
		}
	}

	void copyBack();
};

#endif