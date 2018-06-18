all: tsp

tsp: tsp.cpp
	g++ -std=c++14 tsp.cpp -o tsp
