#ifndef CN_Solver
#define CN_Solver

#include "CN_Widget.h"

class Solver{

public:
	Solver(WidgetGraph w){
		this->widgetGraph = w;
	}

	WidgetGraph* widgetGraph;
	void lpsolver(int opMode);
};



#endif
