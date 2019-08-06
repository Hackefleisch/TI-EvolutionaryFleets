#include "Simulation.h"



void Simulation::Initialize(){

	for(Fleet& fleet : fleets){
		fleet.Initialize(9, 3);
		std::cout << fleet.GetName() << std::endl;
	}

	fleets.at(0).Fight(fleets.at(1));
	fleets.at(2).Fight(fleets.at(3));

}

Simulation::Simulation()
	:
	rng(rd()),
	d10(1, 10),
	d1000(1, 1000)
{
	for(int i = 0; i < 5; i++){
		fleets.emplace_back(&rng, &d1000, &d10);
	}
}


Simulation::~Simulation(){}
