#include "CN_Executer.h"

#include <vector>
#include <algorithm>

void Executer::execute(vector<Edge*>& path, Node* src, Node* dest){
	for (int i = 0; i < path.size(); ++i){
		path[i]->setCurrent(path[i]->get_d_current() == 1 ? 0 : 1);
	}
	for (int i = path.size() - 2; i >= 0; --i){
		path[i]->set_interest_rate(
			max(path[i]->get_interest_rate(), path[i+1]->get_interest_rate()));
	}
}