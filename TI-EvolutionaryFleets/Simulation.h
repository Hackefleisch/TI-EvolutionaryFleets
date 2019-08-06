#pragma once

#include "Fleet.h"
#include <random>
#include <vector>
#include <iostream>

class Simulation{
public:
	void Initialize();
	Simulation();
	~Simulation();
private:
	std::vector<Fleet> fleets;
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> d10;
	std::uniform_int_distribution<int> d1000;
};

