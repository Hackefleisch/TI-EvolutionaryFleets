#pragma once

#include "Fleet.h"
#include <random>
#include <vector>
#include <iostream>
#include "BattleReport.h"

class Simulation{
public:
	void Initialize(bool verbose);
	float ScoreFleets(bool verbose);
	void Start(bool verbose);
	void PrintScores() const;
	void EvolvePopulation();
	Simulation();
	~Simulation();
	Fleet FindBestFleet() const;
private:
	float CalcSurvivalChance(int index) const;
private:
	std::vector<Fleet> fleets;
	std::random_device rd;
	std::mt19937 rng;
	std::uniform_int_distribution<int> d10;
	std::uniform_int_distribution<int> d1000;
	// parameters
	float startingRessources = 9.0f;
	int startingFleetSize = 3;
	float maxRessources = 40.0f;
	int maxFleetSize = 10;
	int populationSize = 200;
	int maxIterations = 500;
	float minFitnessImprovement = 0.0005f;
	int nGenerationsToFailMinImprovement = 15;
	float winScoreFactor = 0.0f;
	float dstTotalScoreFactor = 0.0f;
	float tknTotalScoreFactor = 0.0f;
	float dstRelScoreFactor = 1.0f;
	float tknRelScoreFactor = 0.0f;
	float frctScoreFactor = 0.0f;
	float absFrctScoreFactor = 0.0f;
	float mutationRate = 0.1f;
	int mutationIntensity = 3;
	int nScoringEncounters = 15;
	int nEncounterRepitions = 5;
	bool randomizedDeath = true;
	float deathRate = 0.5f;
	float minSurvivalChance = 0.01f;
	float maxSurvivalChance = 0.99f;
	int shipMutationLikelihood = 1;
	int costMutationLikelihood = 4;
	int sizeMutationLikelihood = 4;
};

