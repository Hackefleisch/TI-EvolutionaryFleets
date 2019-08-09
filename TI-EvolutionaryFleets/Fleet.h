#pragma once

#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <assert.h>
#include <algorithm>

#include "BattleReport.h"
#include "Ship.h"

class Fleet{
public:
	Fleet() = default;
	Fleet(std::mt19937* in_rng, std::uniform_int_distribution<int>* in_d1000, std::uniform_int_distribution<int>* in_d10);
	Fleet(const Fleet& in_fleet);
	Fleet& operator=(const Fleet& in_fleet);
	~Fleet() = default;
	void Initialize(float in_costLimit, int in_capitalShipLimit);
	std::string GetName() const;
	void Reset();
	void Refresh();
	BattleReport Fight(Fleet& opposingFleet, bool verbose);
	void SetFitness(float score);
	float GetFitness() const;
	float GetCost() const;
	int GetSize() const;
	std::vector<int> GetTypeCount() const;
	bool operator<(const Fleet& fleet) const;
	bool IsDead() const;
	bool CanReproduce() const;
	void MarkDead();
	void Reproduce(const Fleet & fleet, float mutationChance, int mutationIntensity, int shipMutationLikelihood, int costMutationLikelihood,
				   int sizeMutationLikelihood, int maxFleetSize, float maxRessources);
private:
	int UpdateBuyableList();
	int AssignHitsToType(int nHits, ShipTypes type, bool verbose);
	int SustainDamage(int nHits, bool verbose);
	int ProduceBarrageHits(bool verbose) const;
	int ProduceHits(bool verbose) const;
	bool CanFight() const;
	void AssignHits(int nHits, bool verbose);
	float CountActiveRessource() const;
	void BuyShips();
	ShipTypes GetTypeToBuy(int nListItems) const;
	void BuyShipType(ShipTypes type);
	bool CanBuyType(ShipTypes type) const;
	void PreBuyShips(std::vector<ShipTypes> preBuyList);
	void UpdateName();
private:
	bool dead = false;
	bool canReproduce = true;
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
	float fitness = 0.0f;
	std::mt19937* rng;
	std::uniform_int_distribution<int>* d10;
	std::uniform_int_distribution<int>* d1000;
};

