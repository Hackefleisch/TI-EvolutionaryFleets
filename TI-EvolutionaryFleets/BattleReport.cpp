#include "BattleReport.h"

std::string BattleReport::GetString() const{

	std::stringstream oCC;
	oCC << std::fixed << std::setprecision(1) << enemyCombinedCost;
	std::string str = "Attacker of combined cost " + oCC.str();
	if(battleWon){
		str += " won ";
	} else{
		str += " lost ";
	}
	std::stringstream eCC;
	eCC << std::fixed << std::setprecision(1) << enemyCombinedCost;
	str += "against enemy with " + eCC.str() + " combined cost.";
	std::stringstream rD;
	rD << std::fixed << std::setprecision(1) << ressourcesDetroyed;
	str += " Attacker destroyed " + rD.str() + " ressources and lost ";
	std::stringstream rL;
	rL << std::fixed << std::setprecision(1) << ressourcesLost;
	str += rL.str() + ".";

	return str;
}
