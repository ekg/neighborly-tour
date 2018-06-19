all: neighborly-tour

neighborly-tour: neighborly-tour.cpp
	g++ -std=c++11 -O3 neighborly-tour.cpp -o neighborly-tour
