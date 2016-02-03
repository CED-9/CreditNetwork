#include "include/rapidjson/document.h"     // rapidjson's DOM-style API
#include "include/rapidjson/prettywriter.h" // for stringify JSON
#include "include/rapidjson/filewritestream.h"
#include "include/rapidjson/filereadstream.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace rapidjson;
using namespace std;

struct Config {
	string creditCapacity;
	string requestAmount;

	string numNodes;
	string edgeProb;
	string transVal;
	string window;
	string smoothing;
	string numIR;
	vector<string> assignedStrategy;
};

struct PlayerInfo {
	double payoff;
	string strategy;
	string role;
	int features[10];
};

void readConfig (Config &config, string inPath) {
	cout<<"trying to read"<<endl;
	FILE* fp = fopen(inPath.c_str(), "r"); // non-Windows use "r"
	cout<<"fopened"<<endl;
	char readBuffer[65536];
	cout << "buffer set" << endl;
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	cout << "stream set" << endl;
	Document doc;
	doc.ParseStream(is);

	printf("\nModified JSON with reformatting:\n");
	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);
	doc.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
	puts(sb.GetString());

	const Value& a = doc["assignment"];
	cout << "hehe" << endl;
	const Value& b = a["All"];

	cout << "size: " << b.Size() << endl;
	// rapidjson uses SizeType instead of size_t.
	for (rapidjson::SizeType i = 0; i < b.Size(); i++)
	{
		config.assignedStrategy.push_back(b[i].GetString());
		printf("%s \n", b[i].GetString());
	}
	
	const Value& configObj = doc["configuration"];
	cout << "haha" << endl;
	config.numNodes = configObj["numNodes"].GetString();
	cout << "haha" << endl;
	config.edgeProb = configObj["edgeProb"].GetString();
	cout << "haha" << endl;
	config.transVal = configObj["transVal"].GetString();
	cout << "haha" << endl;
	config.window = configObj["window"].GetString();
	cout << "haha" << endl;
	config.smoothing = configObj["smoothing"].GetString();
	cout << "haha" << endl;
	config.numIR = configObj["numIR"].GetString();
	cout << "haha" << endl;
	config.creditCapacity = configObj["creditCapacity"].GetString();
	cout << "haha" << endl;
	config.requestAmount = configObj["requestAmount"].GetString();
	cout << "haha8" << endl;
	

	fclose(fp);
	cout<<"fclosed"<<endl;
	return;
}


#include "CN_CreditNet.h"

int main(int argc, char* argv[]){
	
	std::string json_folder = argv[1];
	cout<<json_folder<<endl;
	int num_obs = atoi(argv[2]);
	cout <<  num_obs << endl;
	Config config;
	cout << "configed" << endl;
	readConfig(config, json_folder+"/simulation_spec.json");
	cout << "spec read" << endl;


	return 0;
}   
   