#include "Simulation.h"



void Simulation::Initialize(bool verbose){

	for(Fleet& fleet : fleets){
		fleet.Initialize(startingRessources, startingFleetSize);
		if(verbose)std::cout << fleet.GetName() << std::endl;
	}

}

float Simulation::ScoreFleets(bool verbose){
	float meanFitness = 0.0f;
	std::uniform_int_distribution<int> enemySelector(0, populationSize - 1);

	for(int i = 0; i < populationSize; i++){

		Fleet& scoringFleet = fleets.at(i);

		float winScore = 0.0f;
		float dstTotalScore = 0.0f;
		float tknTotalScore = 0.0f;
		float dstRelScore = 0.0f;
		float tknRelScore = 0.0f;
		float frctScore = 0.0f;
		float fitness = 0.0f;

		for(int j = 0; j < nScoringEncounters; j++){
			int enemyIndex;
			do{
				enemyIndex = enemySelector(rng);
			} while(enemyIndex == i);

			Fleet& enemyFleet = fleets.at(enemyIndex);

			for(int k = 0; k < nEncounterRepitions; k++){
				BattleReport br = scoringFleet.Fight(enemyFleet, verbose);
				if(br.battleWon){
					winScore++;
				}
				dstTotalScore += br.ressourcesDetroyed / maxRessources;
				tknTotalScore += br.ressourcesLost / maxRessources;
				dstRelScore += br.ressourcesDetroyed / br.enemyCombinedCost;
				tknRelScore += br.ressourcesLost / br.ownCombinedCost;
				frctScore += br.ressourcesDetroyed / (br.ownCombinedCost * maxRessources);
			}
		}

		// normalize
		int nBattles = nScoringEncounters * nEncounterRepitions;
		winScore /= nBattles;
		dstTotalScore /= nBattles;
		tknTotalScore /= nBattles;
		dstRelScore /= nBattles;
		tknRelScore /= nBattles;
		frctScore /= nBattles;

		//invert negative scores
		tknTotalScore = 1 - tknTotalScore;
		tknRelScore = 1 - tknRelScore;

		// combine
		fitness = winScore * winScoreFactor + dstTotalScore * dstTotalScoreFactor + tknTotalScore * tknTotalScoreFactor + dstRelScore * dstRelScoreFactor + tknRelScore * tknRelScoreFactor + frctScore * frctScoreFactor;
		// normalize
		fitness /= winScoreFactor + dstTotalScoreFactor + tknTotalScoreFactor + dstRelScoreFactor + tknRelScoreFactor + frctScoreFactor;

		scoringFleet.SetFitness(fitness);
		meanFitness += fitness / populationSize;
	}
	
	return meanFitness;
}

void Simulation::PrintScores() const{
	for(const Fleet& fleet : fleets){
		std::cout << fleet.GetName() + "   " + std::to_string(fleet.GetFitness()) << std::endl;
	}
}

Simulation::Simulation()
	:
	rng(rd()),
	d10(1, 10),
	d1000(1, 1000)
{
	fleets.reserve(populationSize);
	for(int i = 0; i < populationSize; i++){
		fleets.emplace_back(&rng, &d1000, &d10);
	}
}


Simulation::~Simulation(){}
