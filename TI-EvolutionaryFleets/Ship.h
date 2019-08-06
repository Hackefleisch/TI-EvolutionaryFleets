#pragma once

#include <random>
#include "ShipTypes.h"
#include <iostream>

class Ship{
public:
	Ship() = default;
	Ship(std::mt19937* in_rng, std::uniform_int_distribution<int>* in_d10);
	void MakeOfType(ShipTypes in_type);
	int ProduceHits(bool verbose) const;
	int ProduceBarrageHits(bool verbose) const;
	bool TakeHit(bool verbose);
	void Refresh();
	bool CanSustain() const;
	ShipTypes GetType() const;
	bool IsDestroyed() const;
	float GetCost() const;
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

