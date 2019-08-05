#include "Simulation.h"
#include <iostream>



void Simulation::Initialize(){

	fleet.Initialize(9, 3);
	std::cout << fleet.GetName() << std::endl;
	fleet.Initialize(9, 3);
	std::cout << fleet.GetName() << std::endl;

}

Simulation::Simulation()
	:
	rng(rd()),
	d10(1, 10),
	d1000(1, 1000),
	fleet(&rng, &d1000, &d10)
{}


Simulation::~Simulation(){}
