all: neighborly-tour

tsp: neighborly-tour.cpp
	g++ -std=c++14 -O3 neighborly-tour.cpp -o neighborly-tour
