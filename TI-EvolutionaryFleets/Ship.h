#pragma once

#include <random>
#include "ShipTypes.h"
#include <iostream>

class Ship{
public:
	Ship() = default;
	Ship(std::mt19937* in_rng, std::uniform_int_distribution<int>* in_d10);
	void MakeOfType(ShipTypes in_type);
	int ProduceHits() const;
	int ProduceBarrageHits() const;
	bool TakeHit();
	void Refresh();
	bool CanSustain() const;
	ShipTypes GetType() const;
	bool IsDestroyed() const;
private:
	ShipTypes type;
	float cost;
	int combatRoll;
	int combatDice;
	int capacity;
	int barrageRoll;
	int barrageDice;
	bool sustainDamage;
	bool sustainedDamage = false;
	bool destroyed = false;
	std::mt19937* rng;
	std::uniform_int_distribution<int>* d10;
};

