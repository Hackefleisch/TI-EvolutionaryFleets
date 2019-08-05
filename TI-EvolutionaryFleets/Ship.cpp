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
	int hits = 0;
	for(int i = 0; i < combatDice; i++){
		int d10result = (*d10)(*rng);
		if(d10result >= combatRoll){
			hits++;
		}
	}
	return hits;
}

int Ship::ProduceBarrageHits() const{
	int barrageHits = 0;

	for(int i = 0; i < barrageDice; i++){
		int d10result = (*d10)(*rng);
		if(d10result >= barrageRoll){
			barrageHits++;
		}
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
			sustainedDamage = true;
		} else{
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
