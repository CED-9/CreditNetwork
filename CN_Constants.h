#ifndef CN_Const
#define CN_Const

#include <vector>

using namespace std;

class CredNetConstants{

public:
	vector<double> totalIrs;

	CredNetConstants(){}

	void addIr(double ir);

	void print();
};


#endif