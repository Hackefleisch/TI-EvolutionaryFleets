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
	// poorly written and might be wrong
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

Fleet & Fleet::operator=(const Fleet & in_fleet){
	// poorly written and might be wrong
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
	return *this;
}

void Fleet::Initialize(float in_costLimit, int in_capitalShipLimit){

	Reset();

	costLimit = in_costLimit;
	capitalShipLimit = in_capitalShipLimit;

	BuyShips();

	UpdateName();
}

std::string Fleet::GetName() const{

	return name;
}

void Fleet::Reset(){

	dead = false;
	canReproduce = true;
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
	canReproduce = true;
	dead = false;
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

void Fleet::SetFitness(float score){

	fitness = score;

}

float Fleet::GetFitness() const{
	return fitness;
}

bool Fleet::operator<(const Fleet & fleet) const{
	return fitness < fleet.fitness;
}

bool Fleet::IsDead() const{
	return dead;
}

bool Fleet::CanReproduce() const{
	return canReproduce;
}

void Fleet::MarkDead(){
	assert(!dead);
	dead = true;
	canReproduce = false;
}

void Fleet::Reproduce(const Fleet & fleet, float mutationChance, int mutationIntensity, int maxFleetSize, float maxRessources){

	Reset();
	canReproduce = false;
	capitalShipLimit = fleet.capitalShipLimit;
	costLimit = fleet.costLimit;

	int maxMutationRoll = (int)(1000 * mutationChance);
	int d1000result = (*d1000)(*rng);
	if(d1000result <= maxMutationRoll){

		std::vector<ShipTypes> prebuyList;
		for(const Ship& ship : fleet.ships){
			prebuyList.push_back(ship.GetType());
		}
		std::random_shuffle(prebuyList.begin(), prebuyList.end());

		int d10result = (*d10)(*rng);
		if(d10result > 5){
			// mutate ship
			d10result = (*d10)(*rng);
			if(d10result > 5){
				int nListItems = UpdateBuyableList();
				ShipTypes type = GetTypeToBuy(nListItems);
				prebuyList.push_back(type);
			} else{
				prebuyList.pop_back();
			}
		}

		d10result = (*d10)(*rng);
		if(d10result > 5){
			// mutate cost
			d1000result = 0;
			// reroll die for strongest result
			for(int i = 0; i < mutationIntensity; i++){
				int tmp = (*d1000)(*rng);
				if(tmp > d1000result){
					d1000result = tmp;
				}
			}
			// calculate intensity
			float change = 0.0f;
			if(d1000result < 533){
				change = 1.0f;
			} else if(d1000result < 800){
				change = 2.0f;
			} else if(d1000result < 933){
				change = 3.0f;
			} else{
				change = 4.0f;
			}
			// calculate direction
			d10result = (*d10)(*rng);
			if(d10result > 5){
				change *= -1;
			}
			// apply change
			costLimit += change;
			if(costLimit > maxRessources){
				costLimit = maxRessources;
			} else if(costLimit < 1.0f){
				costLimit = 1.0f;
			}
		}
		
		d10result = (*d10)(*rng);
		if(d10result > 5){
			// mutate size
			d1000result = 0;
			// reroll die for strongest result
			for(int i = 0; i < mutationIntensity; i++){
				int tmp = (*d1000)(*rng);
				if(tmp > d1000result){
					d1000result = tmp;
				}
			}
			// calculate intensity
			int change = 0;
			if(d1000result < 666){
				change = 1;
			} else{
				change = 2;
			}
			// calculate direction
			d10result = (*d10)(*rng);
			if(d10result > 5){
				change *= -1;
			}
			capitalShipLimit += change;
			if(capitalShipLimit > maxFleetSize){
				capitalShipLimit = maxFleetSize;
			} else if(capitalShipLimit < 1){
				capitalShipLimit = 1;
			}
		}
		
		PreBuyShips(prebuyList);
		BuyShips();
		UpdateName();
	} else{
		combinedCost = fleet.combinedCost;
		capitalShipCount = fleet.capitalShipCount;
		combinedCapacity = fleet.combinedCapacity;
		usedCapacity = fleet.usedCapacity;
		name.assign(fleet.name);
		ships = fleet.ships;
		buyableList = fleet.buyableList;
		typeCount = fleet.typeCount;
		fitness = fleet.fitness;
		rng = fleet.rng;
		d10 = fleet.d10;
		d1000 = fleet.d1000;
	}


}

int Fleet::UpdateBuyableList(){

	int nBuyableItems = 0;

	buyableList.clear();

	for(int t = (int)ShipTypes::Fighter; t <= (int)ShipTypes::WarSun; t++){
		ShipTypes st = static_cast<ShipTypes>(t);
		if(CanBuyType(st)){
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

void Fleet::BuyShips(){

	int nListItems = UpdateBuyableList();

	while(nListItems > 0){
		ShipTypes type = GetTypeToBuy(nListItems);

		BuyShipType(type);

		nListItems = UpdateBuyableList();
	}

}

ShipTypes Fleet::GetTypeToBuy(int nListItems) const{

	int d1000Result = (*d1000)(*rng) - 1;
	int index = d1000Result * nListItems / 1000;

	return buyableList.at(index);

}

void Fleet::BuyShipType(ShipTypes type){

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

}

bool Fleet::CanBuyType(ShipTypes type) const{

	int capacityLeft = combinedCapacity - usedCapacity;
	float ressourcesLeft = costLimit - combinedCost;
	int capitalShipsLeft = capitalShipLimit - capitalShipCount;

	if(type == ShipTypes::Fighter){
		return capacityLeft > 0 && ressourcesLeft >= ShipStats::Costs(ShipTypes::Fighter);
	} else{
		return capitalShipsLeft > 0 && ressourcesLeft >= ShipStats::Costs(type);
	}

}

void Fleet::PreBuyShips(std::vector<ShipTypes> preBuyList){

	std::vector<ShipTypes> tmpList;
	bool buyableLeft = true;

	while((!preBuyList.empty() || !tmpList.empty()) && buyableLeft){

		ShipTypes type = preBuyList.back();
		preBuyList.pop_back();
		if(CanBuyType(type)){
			BuyShipType(type);
		} else{
			tmpList.push_back(type);
		}

		if(preBuyList.empty()){
			preBuyList = tmpList;
			tmpList.clear();
			buyableLeft = false;
			for(ShipTypes t : preBuyList){
				if(CanBuyType(t)){
					buyableLeft = true;
					break;
				}
			}
		}
	}

}

void Fleet::UpdateName(){

	name = "Fi" + std::to_string(typeCount.at((int)ShipTypes::Fighter));
	name += "De" + std::to_string(typeCount.at((int)ShipTypes::Destroyer));
	name += "Cr" + std::to_string(typeCount.at((int)ShipTypes::Cruiser));
	name += "Ca" + std::to_string(typeCount.at((int)ShipTypes::Carrier));
	name += "Dr" + std::to_string(typeCount.at((int)ShipTypes::Dreadnought));
	name += "Ws" + std::to_string(typeCount.at((int)ShipTypes::WarSun));

}
