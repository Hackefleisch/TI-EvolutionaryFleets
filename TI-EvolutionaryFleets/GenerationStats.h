#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#include "ShipTypes.h"
#include "Fleet.h"

class GenerationStats{
public:
	GenerationStats();
	~GenerationStats() = default;
	void UpdateStats(const std::vector<Fleet>& fleets, bool onlyAlive);
	std::string ComposeReport(const GenerationStats oldStats) const;
	float GetMeanFitness() const;
private:
	float meanFitness = 0.0f;
	float maxFitness = 0.0f;
	float meanCost = 0.0f;
	float maxCost = 0.0f;
	float meanSize = 0.0f;
	float maxSize = 0.0f;
	std::vector<float> meanShipTypeCount;
	std::vector<float> maxShipTypeCount;
	std::string nameBestFleet;
	// TODO: include demographics about species sizes in each population
};

