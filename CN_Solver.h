#ifndef CN_Solver
#define CN_Solver

#include "CN_Widget.h"



class CplexGraphConverter{
public:
	int* head;
	int* tail;

	CplexGraphConverter(){}
	~CplexGraphConverter(){
		delete [] supply;
		delete [] head;
		delete [] tail;
		delete [] obj;
		delete [] ub;
		delete [] lb;
	}
};

class Solver{

public:
	Solver(WidgetGraph w){
		this->widgetGraph = w;
	}

	WidgetGraph* widgetGraph;
	void lpsolver(int opMode);
};



#endif
