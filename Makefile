all: CN_Edge.cpp CN_Node.cpp CN_Graph.cpp test.cpp
	g++ -g -c CN_Node.cpp -std=c++11
	g++ -g -c CN_Edge.cpp -std=c++11
	g++ -g -c CN_Graph.cpp -std=c++11
	g++ -g -c CN_WidgetGraph.cpp -std=c++11
	g++ -g -c CN_Searcher.cpp -std=c++11
	g++ -g -c CN_Executer.cpp -std=c++11
	g++ -g -c CN_CreditNet.cpp -std=c++11
	g++ -o test test_widget.cpp *.o -std=c++11
	./test