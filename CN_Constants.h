#ifndef CN_Const
#define CN_Const

#include <vector>
#include <random>
#include <algorithm>

using namespace std;

class CredNetConstants{

public:
	vector<double> totalIrs;

	default_random_engine gloabalGenerator;
	uniform_int_distribution<int> uniformIntDistribution;
	uniform_real_distribution<double> uniformDoubleDistribution;


	CredNetConstants(): uniformIntDistribution(0, 9999), uniformDoubleDistribution(0.0, 1.0){}

	void addIr(double ir);
	void clean();

	void print();
};


#endif