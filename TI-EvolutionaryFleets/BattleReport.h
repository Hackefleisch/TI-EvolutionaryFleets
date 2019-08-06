#pragma once

#include <string>
#include <iomanip>
#include <sstream>

class BattleReport{
public:
	BattleReport() = default;
	~BattleReport() = default;
	std::string GetString() const;
public:
	bool battleWon = false;
	float ressourcesDetroyed = 0.0f;
	float ressourcesLost = 0.0f;
	float enemyCombinedCost = 0.0f;
};

