#pragma once

#include "Ship.h"
#include <vector>
#include <random>
#include <string>
#include <iostream>

class Fleet{
public:
	Fleet() = default;
	Fleet(std::mt19937* in_rng, std::uniform_int_distribution<int>* in_d1000, std::uniform_int_distribution<int>* in_d10);
	Fleet(const Fleet& in_fleet);
	Fleet& operator=(const Fleet&);
	~Fleet() = default;
	void Initialize(float in_costLimit, int in_capitalShipLimit);
	std::string GetName() const;
	void Reset();
	void Refresh();
	void Fight(Fleet& opposingFleet);
private:
	int UpdateBuyableList();
	int AssignHitsToType(int nHits, ShipTypes type);
	int SustainDamage(int nHits);
	int ProduceBarrageHits() const;
	int ProduceHits() const;
	bool CanFight() const;
	void AssignHits(int nHits);
private:
	float costLimit = 0.0f;
	float combinedCost = 0.0f;
	int capitalShipLimit = 0;
	int capitalShipCount = 0;
	int combinedCapacity = 0;
	int usedCapacity = 0;
	std::string name;
	std::vector<Ship> ships;
	std::vector<ShipTypes> buyableList;
	std::vector<int> typeCount;
	float winScore = 0.0f;
	float dstTotalScore = 0.0f;
	float tknTotalScore = 0.0f;
	float dstRelScore = 0.0f;
	float tknRelScore = 0.0f;
	float frctScore = 0.0f;
	std::mt19937* rng;
	std::uniform_int_distribution<int>* d10;
	std::uniform_int_distribution<int>* d1000;
};

