#include "Fleet.h"


Fleet::Fleet(std::mt19937 * in_rng, std::uniform_int_distribution<int>* in_d1000, std::uniform_int_distribution<int>* in_d10){

	rng = in_rng;
	d10 = in_d10;
	d1000 = in_d1000;

	for(int t = (int)ShipTypes::Fighter; t <= (int)ShipTypes::WarSun; t++){
		typeCount.push_back(0);
	}

}

Fleet::Fleet(const Fleet& in_fleet){

	costLimit = in_fleet.costLimit;
	combinedCost = in_fleet.combinedCost;
	capitalShipLimit = in_fleet.capitalShipLimit;
	capitalShipCount = in_fleet.capitalShipCount;
	combinedCapacity = in_fleet.combinedCapacity;
	usedCapacity = in_fleet.usedCapacity;
	name.assign(in_fleet.name);
	ships = in_fleet.ships;
	buyableList = in_fleet.buyableList;
	typeCount = in_fleet.typeCount;
	fitness = in_fleet.fitness;
	rng = in_fleet.rng;
	d10 = in_fleet.d10;
	d1000 = in_fleet.d1000;

}

void Fleet::Initialize(float in_costLimit, int in_capitalShipLimit){

	Reset();

	costLimit = in_costLimit;
	capitalShipLimit = in_capitalShipLimit;

	int nListItems = UpdateBuyableList();

	while(nListItems > 0){
		int d1000Result = (*d1000)(*rng) - 1;
		int index = d1000Result * nListItems / 1000;

		ShipTypes type = buyableList.at(index);

		Ship tmp(rng, d10);
		tmp.MakeOfType(type);
		ships.push_back(tmp);

		combinedCost += ShipStats::Costs(type);
		combinedCapacity += ShipStats::Capacity(type);
		typeCount.at((int)type)++;

		if(type == ShipTypes::Fighter){
			usedCapacity++;
		} else{
			capitalShipCount++;
		}


		nListItems = UpdateBuyableList();
	}

	name = "Fi" + std::to_string(typeCount.at((int)ShipTypes::Fighter));
	name += "De" + std::to_string(typeCount.at((int)ShipTypes::Destroyer));
	name += "Cr" + std::to_string(typeCount.at((int)ShipTypes::Cruiser));
	name += "Ca" + std::to_string(typeCount.at((int)ShipTypes::Carrier));
	name += "Dr" + std::to_string(typeCount.at((int)ShipTypes::Dreadnought));
	name += "Ws" + std::to_string(typeCount.at((int)ShipTypes::WarSun));
}

std::string Fleet::GetName() const{

	return name;
}

void Fleet::Reset(){

	costLimit = 0.0f;
	combinedCost = 0.0f;
	capitalShipLimit = 0;
	capitalShipCount = 0;
	combinedCapacity = 0;
	usedCapacity = 0;
	name = "";
	ships.clear();
	buyableList.clear();
	for(int t = (int)ShipTypes::Fighter; t <= (int)ShipTypes::WarSun; t++){
		typeCount.at(t) = 0;
	}
	fitness = 0.0f;

}

void Fleet::Refresh(){

	for(Ship& ship : ships){
		ship.Refresh();
	}

}

BattleReport Fleet::Fight(Fleet & opposingFleet, bool verbose){

	BattleReport br;

	// Anti Fighter Barrage
	if(verbose){
		std::cout << "\n--------------------------------------" << std::endl;
		std::cout << name + " attacks " + opposingFleet.name << std::endl;
		std::cout << "--------------------------------------\n" << std::endl;
		std::cout << "Anti-Fighter Barrage Attacker:" << std::endl;
	}
	int nOwnBarrageHits = ProduceBarrageHits(verbose);
	if(verbose)std::cout << "Anti-Fighter Barrage Defender:" << std::endl;
	int nEneBarrageHits = opposingFleet.ProduceBarrageHits(verbose);
	if(verbose)std::cout << "Anti-Fighter Barrage Losses Attacker:" << std::endl;
	AssignHitsToType(nEneBarrageHits, ShipTypes::Fighter, verbose);
	if(verbose)std::cout << "Anti-Fighter Barrage Losses Defender:" << std::endl;
	opposingFleet.AssignHitsToType(nOwnBarrageHits, ShipTypes::Fighter, verbose);

	// Repeat combat until one side is eliminated
	while(CanFight() && opposingFleet.CanFight()){
		// produce hits
		if(verbose)std::cout << "\nHits Attacker:" << std::endl;
		int nOwnHits = ProduceHits(verbose);
		if(verbose)std::cout << "Hits Defender:" << std::endl;
		int nEneHits = opposingFleet.ProduceHits(verbose);

		// assign hits
		if(verbose)std::cout << "Losses Attacker:" << std::endl;
		AssignHits(nEneHits, verbose);
		if(verbose)std::cout << "Losses Defender:" << std::endl;
		opposingFleet.AssignHits(nOwnHits, verbose);
	}

	// a battle only counts as victory if the attacking fleet is still able to press on
	br.battleWon = CanFight();
	br.ownCombinedCost = combinedCost;
	br.enemyCombinedCost = opposingFleet.combinedCost;
	br.ressourcesDetroyed = opposingFleet.combinedCost - opposingFleet.CountActiveRessource();
	br.ressourcesLost = combinedCost - CountActiveRessource();

	// Refresh all ships
	Refresh();
	opposingFleet.Refresh();

	return br;
}

int Fleet::UpdateBuyableList(){

	int nBuyableItems = 0;

	buyableList.clear();

	int capacityLeft = combinedCapacity - usedCapacity;
	float ressourcesLeft = costLimit - combinedCost;
	int capitalShipsLeft = capitalShipLimit - capitalShipCount;

	if(capacityLeft > 0 && ressourcesLeft >= ShipStats::Costs(ShipTypes::Fighter)){
		buyableList.push_back(ShipTypes::Fighter);
		nBuyableItems++;
	}

	for(int t = (int)ShipTypes::Destroyer; t != (int)ShipTypes::WarSun; t++){
		ShipTypes st = static_cast<ShipTypes>(t);
		if(capitalShipsLeft > 0 && ressourcesLeft >= ShipStats::Costs(st)){
			buyableList.push_back(st);
			nBuyableItems++;
		}
	}

	return nBuyableItems;
}

int Fleet::AssignHitsToType(int nHits, ShipTypes type, bool verbose){
	// This function should assign as many hits as possible to ships of type and return the amount of hits left
	for(Ship& ship : ships){
		if(ship.GetType() == type && nHits > 0){
			if(ship.TakeHit(verbose)){
				nHits--;
			}
		}
	}

	return nHits;
}

int Fleet::SustainDamage(int nHits, bool verbose){
	// This function tries to cancel as many hits as possible with sustain damage and returns the amount of hits left
	for(Ship& ship : ships){
		if(ship.CanSustain() && nHits > 0){
			if(ship.TakeHit(verbose)){
				nHits--;
			}
		}
	}

	return nHits;
}

int Fleet::ProduceBarrageHits(bool verbose) const{
	int nHits = 0;
	for(const Ship& ship : ships){
		nHits += ship.ProduceBarrageHits(verbose);
	}
	return nHits;
}

int Fleet::ProduceHits(bool verbose) const{
	int nHits = 0;
	for(const Ship& ship : ships){
		nHits += ship.ProduceHits(verbose);
	}
	return nHits;
}

bool Fleet::CanFight() const{
	for(const Ship& ship : ships){
		if(!ship.IsDestroyed()){
			return true;
		}
	}
	return false;
}

void Fleet::AssignHits(int nHits, bool verbose){

	nHits = SustainDamage(nHits, verbose);
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Fighter, verbose);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Destroyer, verbose);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Carrier, verbose);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Cruiser, verbose);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Dreadnought, verbose);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::WarSun, verbose);
	}
}

float Fleet::CountActiveRessource() const{
	float activeRessources = 0.0f;

	for(const Ship& ship : ships){
		if(!ship.IsDestroyed()){
			activeRessources += ship.GetCost();
		}
	}

	return activeRessources;
}
