#include "GenerationStats.h"

GenerationStats::GenerationStats(){

	for(int i = (int)ShipTypes::Fighter; i <= (int)ShipTypes::WarSun; i++){
		meanShipTypeCount.push_back(0.0f);
		maxShipTypeCount.push_back(0.0f);
	}

}

void GenerationStats::UpdateStats(const std::vector<Fleet>& fleets, bool onlyAlive){

	int fleetCounter = 0;

	meanFitness = 0.0f;
	maxFitness = 0.0f;
	meanCost = 0.0f;
	maxCost = 0.0f;
	meanSize = 0.0f;
	maxSize = 0.0f;

	for(int i = (int)ShipTypes::Fighter; i <= (int)ShipTypes::WarSun; i++){
		meanShipTypeCount.at(i) = 0.0f;
		maxShipTypeCount.at(i) = 0.0f;
	}

	for(const Fleet& fleet : fleets){
		if(!fleet.IsDead() || !onlyAlive){
			fleetCounter++;
			meanFitness += fleet.GetFitness();
			meanCost += fleet.GetCost();
			meanSize += (float)fleet.GetSize();

			if(fleet.GetFitness() > maxFitness){
				maxFitness = fleet.GetFitness();
				nameBestFleet = fleet.GetName();
			}

			if(fleet.GetCost() > maxCost){
				maxCost = fleet.GetCost();
			}

			if(fleet.GetSize() > maxSize){
				maxSize = (float)fleet.GetSize();
			}

			for(int i = (int)ShipTypes::Fighter; i <= (int)ShipTypes::WarSun; i++){
				meanShipTypeCount.at(i) += (float)fleet.GetTypeCount().at(i);
				if((float)fleet.GetTypeCount().at(i) > maxShipTypeCount.at(i)){
					maxShipTypeCount.at(i) = (float)fleet.GetTypeCount().at(i);
				}
			}
		}
	}

	meanFitness /= fleetCounter;
	meanCost /= fleetCounter;
	meanSize /= fleetCounter;

	for(int i = (int)ShipTypes::Fighter; i <= (int)ShipTypes::WarSun; i++){
		meanShipTypeCount.at(i) /= fleetCounter;
	}

}

std::string GenerationStats::ComposeReport(const GenerationStats oldStats) const{

	std::stringstream ss;
	ss << "\tbest fleet " << nameBestFleet << "\n";
	ss << "\tmean fitness " << meanFitness << ", improved by " << meanFitness - oldStats.meanFitness << "\n";
	ss << "\tmax fitness " << maxFitness << ", improved by " << maxFitness - oldStats.maxFitness << "\n";
	ss << "\tmean cost " << meanCost << ", improved by " << meanCost - oldStats.meanCost << "\n";
	ss << "\tmax cost " << maxCost << ", improved by " << maxCost - oldStats.maxCost << "\n";
	ss << "\tmean size " << meanSize << ", improved by " << meanSize - oldStats.meanSize << "\n";
	ss << "\tmax size " << maxSize << ", improved by " << maxSize - oldStats.maxSize << "\n";
	for(int i = (int)ShipTypes::Fighter; i <= (int)ShipTypes::WarSun; i++){
		ShipTypes type = static_cast<ShipTypes>(i);
		ss << "\t\t" << ShipStats::Name(type) << " count mean " << std::fixed << std::setprecision(2) << meanShipTypeCount.at(i);
		ss << " max " << std::fixed << std::setprecision(0) << maxShipTypeCount.at(i) << "\n";
	}

	return ss.str();
}

float GenerationStats::GetMeanFitness() const{
	return meanFitness;
}
