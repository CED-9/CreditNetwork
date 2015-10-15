all: CN_Edge.cpp CN_Node.cpp CN_Graph.cpp test.cpp
	g++ -g -c CN_Node.cpp -std=c++11
	g++ -g -c CN_Edge.cpp -std=c++11
	g++ -g -c CN_Graph.cpp -std=c++11
	# g++ -g -c CN_WidgetGraph.cpp -std=c++11
	g++ -g -c CN_Searcher.cpp -std=c++11
	g++ -g -c CN_Executer.cpp -std=c++11
	g++ -g -c CN_CreditNet.cpp -std=c++11
	g++ -o test test.cpp *.o -std=c++11
	./test

liquid: CN_Edge.cpp CN_Node.cpp CN_Graph.cpp test.cpp
	g++ -g -c CN_Node.cpp -std=c++11
	g++ -g -c CN_Edge.cpp -std=c++11
	g++ -g -c CN_Graph.cpp -std=c++11
	g++ -g -c CN_Searcher.cpp -std=c++11
	g++ -g -c CN_Executer.cpp -std=c++11
	g++ -g -c CN_CreditNet.cpp -std=c++11
	g++ -o test_liquidity test_liquidity.cpp *.o -pthread -std=c++11

liquid2: CN_Edge.cpp CN_Node.cpp CN_Graph.cpp test.cpp
	g++ -g -c CN_Node.cpp -std=c++11
	g++ -g -c CN_Edge.cpp -std=c++11
	g++ -g -c CN_Graph.cpp -std=c++11
	g++ -g -c CN_Searcher.cpp -std=c++11
	g++ -g -c CN_Executer.cpp -std=c++11
	g++ -g -c CN_CreditNet.cpp -std=c++11
	g++ -o test_liquidity2 test_liquidity2.cpp *.o -pthread -std=c++11
	./test_liquidity2 10 1
test:
	./test_liquidity 9 1
	./test_liquidity 9 2
	./test_liquidity 9 3
	./test_liquidity 9 4
	./test_liquidity 9 5
	./test_liquidity 9 6
	./test_liquidity 9 7
	./test_liquidity 9 8
	./test_liquidity 9 9
	./test_liquidity 10 1
	./test_liquidity 10 2
	./test_liquidity 10 3
	./test_liquidity 10 4
	./test_liquidity 10 5
	./test_liquidity 10 6
	./test_liquidity 10 7
	./test_liquidity 10 8
	./test_liquidity 10 9

game: CN_Edge.cpp CN_Node.cpp CN_Graph.cpp test.cpp
	g++ -g -c CN_Node.cpp -std=c++11
	g++ -g -c CN_Edge.cpp -std=c++11
	g++ -g -c CN_Graph.cpp -std=c++11
	g++ -g -c CN_Searcher.cpp -std=c++11
	g++ -g -c CN_Executer.cpp -std=c++11
	g++ -g -c CN_CreditNet.cpp -std=c++11
	g++ -o test_game test_game.cpp *.o -std=c++11
	./test_game