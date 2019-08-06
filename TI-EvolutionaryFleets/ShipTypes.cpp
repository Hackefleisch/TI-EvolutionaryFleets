#include "ShipTypes.h"

float ShipStats::Costs(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return 0.5f;
		case ShipTypes::Destroyer:
			return 1.0f;
		case ShipTypes::Cruiser:
			return 2.0f;
		case ShipTypes::Carrier:
			return 3.0f;
		case ShipTypes::Dreadnought:
			return 4.0f;
		case ShipTypes::WarSun:
			return 12.0f;
		default:
			return -1.0f;
	}
}

int ShipStats::Capacity(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return 0;
		case ShipTypes::Destroyer:
			return 0;
		case ShipTypes::Cruiser:
			return 0;
		case ShipTypes::Carrier:
			return 4;
		case ShipTypes::Dreadnought:
			return 1;
		case ShipTypes::WarSun:
			return 6;
		default:
			return -1;
	}
}

int ShipStats::CombatRoll(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return 9;
		case ShipTypes::Destroyer:
			return 9;
		case ShipTypes::Cruiser:
			return 7;
		case ShipTypes::Carrier:
			return 9;
		case ShipTypes::Dreadnought:
			return 5;
		case ShipTypes::WarSun:
			return 3;
		default:
			return -1;
	}
}

int ShipStats::CombatDice(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return 1;
		case ShipTypes::Destroyer:
			return 1;
		case ShipTypes::Cruiser:
			return 1;
		case ShipTypes::Carrier:
			return 1;
		case ShipTypes::Dreadnought:
			return 1;
		case ShipTypes::WarSun:
			return 3;
		default:
			return -1;
	}
}

int ShipStats::BarrageRoll(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return 0;
		case ShipTypes::Destroyer:
			return 9;
		case ShipTypes::Cruiser:
			return 0;
		case ShipTypes::Carrier:
			return 0;
		case ShipTypes::Dreadnought:
			return 0;
		case ShipTypes::WarSun:
			return 0;
		default:
			return -1;
	}
}

int ShipStats::BarrageDice(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return 0;
		case ShipTypes::Destroyer:
			return 2;
		case ShipTypes::Cruiser:
			return 0;
		case ShipTypes::Carrier:
			return 0;
		case ShipTypes::Dreadnought:
			return 0;
		case ShipTypes::WarSun:
			return 0;
		default:
			return -1;
	}
}

bool ShipStats::SustainDamage(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return false;
		case ShipTypes::Destroyer:
			return false;
		case ShipTypes::Cruiser:
			return false;
		case ShipTypes::Carrier:
			return false;
		case ShipTypes::Dreadnought:
			return true;
		case ShipTypes::WarSun:
			return true;
		default:
			return false;
	}
}

std::string ShipStats::Name(ShipTypes type){
	switch(type){
		case ShipTypes::Fighter:
			return "Fighter";
		case ShipTypes::Destroyer:
			return "Destroyer";
		case ShipTypes::Cruiser:
			return "Cruiser";
		case ShipTypes::Carrier:
			return "Carrier";
		case ShipTypes::Dreadnought:
			return "Dreadnought";
		case ShipTypes::WarSun:
			return "WarSun";
	}
	return "";
}
