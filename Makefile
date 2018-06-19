all: tsp

tsp: tsp.cpp
	g++ -std=c++14 -O3 tsp.cpp -o tsp
