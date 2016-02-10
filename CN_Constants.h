#ifndef CN_Const
#define CN_Const

#include <vector>

using namespace std;

class CredNetConstants{

public:
	vector<double> totalIrs;

	default_random_engine gloabalGenerator;
	uniform_int_distribution<int> unifromIntDistribution(0, 9999);
	uniform_real_distribution<double> uniformDoubleDistribution(0.0,1.0);


	CredNetConstants(){}

	void addIr(double ir);
	void clean();

	void print();
};


#endif