#ifndef CN_Const
#define CN_Const

#include <vector>

using namespace std;

class CredNetConstants{

public:
	vector<double> totalIrs;

	CredNetConstants(){
		totalIrs.push_back(0.1);
		totalIrs.push_back(0.2);
		totalIrs.push_back(0.3);
	}
};

const CredNetConstants credNetConstants;

#endif