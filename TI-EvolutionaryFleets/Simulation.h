#pragma once

#include "Fleet.h"
#include <random>

class Simulation{
public:
	void Initialize();
	Simulation();
	~Simulation();
private:
	Fleet fleet;
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> d10;
	std::uniform_int_distribution<int> d1000;
};

