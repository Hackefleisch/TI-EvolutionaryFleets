#include "Simulation.h"

int main(){

	bool verbose = false;

	Simulation sim;
	sim.Initialize(verbose);
	for(int i = 0; i < 10; i++){
		std::cout << sim.ScoreFleets(verbose) << std::endl;
	}
	//sim.PrintScores();

	return 0;
}