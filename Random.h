#pragma once
//code used from airplane simulator

#include <cstdlib>
#include <ctime>

class Random {

public:

	//initializes the random number generator using time
	Random() {
		srand((unsigned int)time(0));
	}

	//initializes the randon mumber generator using a given number
	Random(int seed) {
		srand(seed);
	}

	//returns a random integer in the range 0 – n
	int next_int(int n) {
		return int(next_double() * n);
	}

	//returns a random double in the range 0 – 1
	double next_double() {
		return double(rand()) / RAND_MAX;
	}

};