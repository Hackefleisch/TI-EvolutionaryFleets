#pragma once


enum class ShipTypes{
	Fighter,
	Destroyer,
	Cruiser,
	Carrier,
	Dreadnought,
	WarSun
};

class ShipStats{
public:
	static float Costs(ShipTypes type);
	static int Capacity(ShipTypes type);
	static int CombatRoll(ShipTypes type);
	static int CombatDice(ShipTypes type);
	static int BarrageRoll(ShipTypes type);
	static int BarrageDice(ShipTypes type);
	static bool SustainDamage(ShipTypes type);
};