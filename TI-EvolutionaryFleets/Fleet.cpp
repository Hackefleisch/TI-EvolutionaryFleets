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
	winScore = in_fleet.winScore;
	dstTotalScore = in_fleet.dstTotalScore;
	tknTotalScore = in_fleet.tknTotalScore;
	dstRelScore = in_fleet.dstRelScore;
	tknRelScore = in_fleet.tknRelScore;
	frctScore = in_fleet.frctScore;
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
		int d1000Result = (*d1000)(*rng);
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
	winScore = 0.0f;
	dstTotalScore = 0.0f;
	tknTotalScore = 0.0f;
	dstRelScore = 0.0f;
	tknRelScore = 0.0f;
	frctScore = 0.0f;

}

void Fleet::Refresh(){

	for(Ship ship : ships){
		ship.Refresh();
	}

}

void Fleet::Fight(Fleet & opposingFleet){

	// Anti Fighter Barrage
	std::cout << "\n--------------------------------------" << std::endl;
	std::cout << name + " attacks " + opposingFleet.name << std::endl;
	std::cout << "--------------------------------------\n" << std::endl;
	std::cout << "Anti-Fighter Barrage Attacker:" << std::endl;
	int nOwnBarrageHits = ProduceBarrageHits();
	std::cout << "Anti-Fighter Barrage Defender:" << std::endl;
	int nEneBarrageHits = opposingFleet.ProduceBarrageHits();
	std::cout << "Anti-Fighter Barrage Losses Attacker:" << std::endl;
	AssignHitsToType(nEneBarrageHits, ShipTypes::Fighter);
	std::cout << "Anti-Fighter Barrage Losses Defender:" << std::endl;
	opposingFleet.AssignHitsToType(nOwnBarrageHits, ShipTypes::Fighter);

	// Repeat combat until one side is eliminated
	while(CanFight() && opposingFleet.CanFight()){
		// produce hits
		std::cout << "\nHits Attacker:" << std::endl;
		int nOwnHits = ProduceHits();
		std::cout << "Hits Defender:" << std::endl;
		int nEneHits = opposingFleet.ProduceHits();

		// assign hits
		std::cout << "Losses Attacker:" << std::endl;
		AssignHits(nEneHits);
		std::cout << "Losses Defender:" << std::endl;
		opposingFleet.AssignHits(nOwnHits);
	}

	// Score?


	// Refresh all ships
	Refresh();
	opposingFleet.Refresh();
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

int Fleet::AssignHitsToType(int nHits, ShipTypes type){
	// This function should assign as many hits as possible to ships of type and return the amount of hits left
	for(Ship& ship : ships){
		if(ship.GetType() == type && nHits > 0){
			if(ship.TakeHit()){
				nHits--;
			}
		}
	}

	return nHits;
}

int Fleet::SustainDamage(int nHits){
	// This function tries to cancel as many hits as possible with sustain damage and returns the amount of hits left
	for(Ship& ship : ships){
		if(ship.CanSustain() && nHits > 0){
			if(ship.TakeHit()){
				nHits--;
			}
		}
	}

	return nHits;
}

int Fleet::ProduceBarrageHits() const{
	int nHits = 0;
	for(const Ship& ship : ships){
		nHits += ship.ProduceBarrageHits();
	}
	return nHits;
}

int Fleet::ProduceHits() const{
	int nHits = 0;
	for(const Ship& ship : ships){
		nHits += ship.ProduceHits();
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

void Fleet::AssignHits(int nHits){

	nHits = SustainDamage(nHits);
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Fighter);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Destroyer);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Carrier);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Cruiser);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::Dreadnought);
	}
	if(nHits > 0){
		nHits = AssignHitsToType(nHits, ShipTypes::WarSun);
	}
}
