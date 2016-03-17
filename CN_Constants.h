#ifndef CN_Constants
#define CN_Constants

#include <vector>
#include <random>
#include <algorithm>
#include <chrono>

// forward declaration
class Graph;
class Edge;
class singleCreditEdge;
class AtomicEdge;
class GraphModifier;

using namespace std;

class CredNetConstants{

public:
	vector<double> totalIrs;

	default_random_engine gloabalGenerator;
	uniform_int_distribution<int> uniformIntDistribution;
	uniform_real_distribution<double> uniformDoubleDistribution;


	CredNetConstants(): uniformIntDistribution(0, 9999), uniformDoubleDistribution(0.0, 1.0)
		, gloabalGenerator(std::chrono::system_clock::now().time_since_epoch().count()){
	}

	void addIr(double ir);
	void clean();

	void print();
};


#endif