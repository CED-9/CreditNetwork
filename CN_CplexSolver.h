#ifndef CN_CplexSolver
#define CN_CplexSolver

#include "CN_WidgetGraph.h"

class CplexConverter{

public:
	// widget graph
	int widgetNodeNum;
	int widgetEdgeNum;

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
		widgetNodeNum = widgetNet->widgetNodes.size();
		widgetEdgeNum = widgetNet->widgetEdges.size();

		nnodes = widgetNodeNum;
		narcs = widgetEdgeNum;

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

	void copyBack(){
		for (int i = 0; i < widgetNet->widgetEdges.size(); ++i){
			if (head[i] > widgetNodeNum || tail[i] > widgetNodeNum) {
				cerr << "error!" << endl;
				continue;
			}
			widgetNet->widgetNodes[head[i]]->edge_out[tail[i]]->curr = x[i];
		}

		delete [] supply;
		delete [] head;
		delete [] tail;
		delete [] obj;
		delete [] ub;
		delete [] lb;

		delete [] x;
		delete [] dj;
		delete [] pi;
		delete [] slack;
	}
};


class CplexSolver{

public:

	int lpSolver(CplexConverter& converter);

};


#endif