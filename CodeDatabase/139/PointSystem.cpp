#include "PointSystem.h"


PointSystem::PointSystem(int moveSlowcost, int moveFastCost,
	int attackShortCost, int attackLongCost, 
	int directSpellCost, int areaSpellCost, int trapSpellCost, int allySpellCost,
	int upgradeSpellCost)
	: moveSlowCost(moveSlowcost), moveFastCost(moveFastCost),
	attackShortCost(attackShortCost), attackLongCost(attackLongCost),
	directSpellCost(directSpellCost), areaSpellCost(areaSpellCost),
	trapSpellCost(trapSpellCost), allySpellCost(allySpellCost), upgradeSpellCost(upgradeSpellCost) { }


int PointSystem::getMoveCost(bool fastMode) const
{
	if (fastMode)
		return getMoveFastCost();
	return getMoveSlowCost();
}


int PointSystem::getAttack(bool longMode) const
{
	if (longMode)
		return getAttackLongCost();
	return getAttackShortCost();
}


int PointSystem::getDirectSpellCost() const
{
	return directSpellCost;
}


int PointSystem::getAreaSpellCost() const
{
	return areaSpellCost;
}


int PointSystem::getTrapSpellCost() const
{
	return trapSpellCost;
}


int PointSystem::getAllySpellCost() const
{
	return allySpellCost;
}


int PointSystem::getUpgradeSpellCost() const
{
	return upgradeSpellCost;
}


int PointSystem::getMoveSlowCost() const
{
	return moveSlowCost;
}


int PointSystem::getMoveFastCost() const
{
	return moveFastCost;
}


int PointSystem::getAttackShortCost() const
{
	return attackShortCost;
}


int PointSystem::getAttackLongCost() const
{
	return attackLongCost;
}