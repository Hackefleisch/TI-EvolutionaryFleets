#include "Simulation.h"

int main(){

	bool verbose = false;

	Simulation sim;
	sim.Initialize(verbose);
	sim.Start(verbose);

	return 0;
}