#include "Fleet.h"


Fleet::Fleet(std::mt19937 * in_rng, std::uniform_int_distribution<int>* in_d1000, std::uniform_int_distribution<int>* in_d10){

	rng = in_rng;
	d10 = in_d10;
	d1000 = in_d1000;

	for(int t = (int)ShipTypes::Fighter; t <= (int)ShipTypes::WarSun; t++){
		typeCount.push_back(0);
	}

}

void Fleet::Initialize(float in_costLimit, int in_capitalShipLimit){

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
