
#include "CN_Constants.h"
#include <iostream>

using namespace std;

CredNetConstants credNetConstants;

void CredNetConstants::print(){
	cout << "possible irs: ";
	for (int i = 0; i < totalIrs.size(); ++i){
		cout << totalIrs[i];
	}
	cout << endl;
}


void CredNetConstants::addIr(double ir){
	totalIrs.push_back(ir);
}

void CredNetConstants::clean(){
	totalIrs.clear();
}