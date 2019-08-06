#include "Ship.h"



Ship::Ship(std::mt19937 * in_rng, std::uniform_int_distribution<int>* in_d10){

	rng = in_rng;
	d10 = in_d10;

}

void Ship::MakeOfType(ShipTypes in_type){

	type = in_type;
	cost = ShipStats::Costs(type);
	combatRoll = ShipStats::CombatRoll(type);
	combatDice = ShipStats::CombatDice(type);
	capacity = ShipStats::Capacity(type);
	barrageRoll = ShipStats::BarrageRoll(type);
	barrageDice = ShipStats::BarrageDice(type);
	sustainDamage = ShipStats::SustainDamage(type);

}

int Ship::ProduceHits() const{
	if(destroyed){
		return 0;
	}
	int hits = 0;
	for(int i = 0; i < combatDice; i++){
		int d10result = (*d10)(*rng);
		std::cout << "\t" + ShipStats::Name(type) + " rolled " + std::to_string(d10result);
		if(d10result >= combatRoll){
			std::cout << " HIT!";
			hits++;
		}
		std::cout << std::endl;
	}
	return hits;
}

int Ship::ProduceBarrageHits() const{
	if(destroyed){
		return 0;
	}
	int barrageHits = 0;

	for(int i = 0; i < barrageDice; i++){
		int d10result = (*d10)(*rng);
		std::cout << "\t" + ShipStats::Name(type) + " rolled " + std::to_string(d10result);
		if(d10result >= barrageRoll){
			std::cout << " HIT!";
			barrageHits++;
		}
		std::cout << std::endl;
	}

	return barrageHits;
}

bool Ship::TakeHit(){
	// returns true if this ship took a hit and false if this ship wasn't able to take more
	// if the ship takes a hit it is properly processed
	if(destroyed){
		return false;
	} else{
		if(CanSustain()){
			std::cout << "\t" + ShipStats::Name(type) + " sustained damage." << std::endl;
			sustainedDamage = true;
		} else{
			std::cout << "\t" + ShipStats::Name(type) + " got destroyed." << std::endl;
			destroyed = true;
		}
		return true;
	}
}

void Ship::Refresh(){

	sustainedDamage = false;
	destroyed = false;

}

bool Ship::CanSustain() const{
	return sustainDamage && !sustainedDamage;
}

ShipTypes Ship::GetType() const{
	return type;
}

bool Ship::IsDestroyed() const{
	return destroyed;
}
