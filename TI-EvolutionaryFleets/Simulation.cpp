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
		float absFrctScore = 0.0f;
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
				absFrctScore += br.ressourcesDetroyed / br.ownCombinedCost;
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
		absFrctScore /= nBattles;

		//invert negative scores
		tknTotalScore = 1 - tknTotalScore;
		tknRelScore = 1 - tknRelScore;

		// combine
		fitness = winScore * winScoreFactor + dstTotalScore * dstTotalScoreFactor + tknTotalScore * tknTotalScoreFactor + dstRelScore * dstRelScoreFactor + tknRelScore * tknRelScoreFactor + frctScore * frctScoreFactor + absFrctScore * absFrctScoreFactor;
		// normalize
		fitness /= winScoreFactor + dstTotalScoreFactor + tknTotalScoreFactor + dstRelScoreFactor + tknRelScoreFactor + frctScoreFactor + absFrctScoreFactor;

		scoringFleet.SetFitness(fitness);
		meanFitness += fitness / populationSize;
	}
	
	return meanFitness;
}

void Simulation::Start(bool verbose){

	float newMeanFitness = 0.0f;
	float meanFitnessDiff = 0.0f;
	float bestMeanFitness = 0.0f;
	int iteration = 0;
	int counterFailedImprovements = 0;

	GenerationStats oldStats;
	GenerationStats newStats;

	do{
		iteration++;

		// Score fleets
		ScoreFleets(verbose);

		// lowest fitness is first, highest last
		std::sort(fleets.begin(), fleets.end());

		int deadCounter = KillPopulation();

		// collect data of current alive population
		newStats.UpdateStats(fleets, onlyAliveStats);

		newMeanFitness = newStats.GetMeanFitness();
			meanFitnessDiff = newMeanFitness - bestMeanFitness;
		if(newMeanFitness > bestMeanFitness){
			bestMeanFitness = newMeanFitness;
		}

		if(meanFitnessDiff < minFitnessImprovement){
			counterFailedImprovements++;
		} else{
			counterFailedImprovements = 0;
		}

		// renew population
		ReproducePopulation(deadCounter);

		// compose report
		std::cout << iteration << ". generation" << std::endl;
		std::cout << newStats.ComposeReport(oldStats);

		oldStats = newStats;

	} while(iteration < maxIterations && counterFailedImprovements <= nGenerationsToFailMinImprovement);

	if(counterFailedImprovements >= nGenerationsToFailMinImprovement){
		std::cout << "Exited because of " << nGenerationsToFailMinImprovement << " failed improvements in a row." << std::endl;
	} else{
		std::cout << "Maximum iterations reached." << std::endl;
	}

}

void Simulation::PrintScores() const{
	for(const Fleet& fleet : fleets){
		std::cout << fleet.GetName() + "   " + std::to_string(fleet.GetFitness()) << std::endl;
	}
}

void Simulation::ReproducePopulation(int deadCounter){

	int deadIndex = 0;
	std::uniform_int_distribution<int> reproStartPicker(0, populationSize - 1);
	int reproduceIndex = reproStartPicker(rng);
	while(deadCounter > 0){
		while(!fleets.at(deadIndex).IsDead()){
			deadIndex++;
			if(deadIndex >= populationSize){
				deadIndex = 0;
			}
		}
		while(!fleets.at(reproduceIndex).CanReproduce()){
			reproduceIndex++;
			if(reproduceIndex >= populationSize){
				reproduceIndex = 0;
			}
		}
		Fleet& deadFleet = fleets.at(deadIndex);
		Fleet& reproduceFleet = fleets.at(reproduceIndex);
		deadFleet.Reproduce(reproduceFleet, mutationRate, mutationIntensity, shipMutationLikelihood, costMutationLikelihood, sizeMutationLikelihood, maxFleetSize, maxRessources);
		deadCounter--;
		reproduceIndex++;
		if(reproduceIndex >= populationSize){
			reproduceIndex = 0;
		}
	}
	
}

int Simulation::KillPopulation(){

	int nDyingFleets = (int)(populationSize * deathRate);
	int deadCounter = 0;

	if(randomizedDeath){
		while(deadCounter < nDyingFleets){

			for(int i = 0; i < populationSize && deadCounter < nDyingFleets; i++){
				Fleet& fleet = fleets.at(i);
				if(!fleet.IsDead()){
					int maxRollToSurvive = (int)(1000 * CalcSurvivalChance(i));
					int d1000result = d1000(rng);
					if(d1000result > maxRollToSurvive){
						// fleet dies
						fleet.MarkDead();
						deadCounter++;
					}
				}
			}

		}
	} else{
		for(int i = 0; i < nDyingFleets; i++){
			fleets.at(i).MarkDead();
			deadCounter++;
		}
	}

	return deadCounter;
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

Fleet Simulation::FindBestFleet() const{
	Fleet bestFleet;
	float bestScore = 0.0f;
	for(int i = 0; i < populationSize; i++){
		if(fleets.at(i).GetFitness() > bestScore){
			bestScore = fleets.at(i).GetFitness();
			bestFleet = fleets.at(i);
		}
	}
	return bestFleet;
}

float Simulation::CalcSurvivalChance(int index) const{

	float stepsize = (maxSurvivalChance - minSurvivalChance) / populationSize;

	return minSurvivalChance + stepsize * index;
}
