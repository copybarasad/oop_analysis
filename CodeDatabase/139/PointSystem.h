#pragma once


class PointSystem
{
private:

	int moveSlowCost;
	int moveFastCost;
	int attackShortCost;
	int attackLongCost;
	int directSpellCost;
	int areaSpellCost;
	int trapSpellCost;
	int allySpellCost;
	int upgradeSpellCost;
public:
	PointSystem(int moveSlowcost = 1, int moveFastCost = -2, 
		int attackShortCost = -3, int attackLongCost = -5, 
		int directSpellCost = -7, int areaSpellCost = -12,
		int trapSpellCost = -5, int allySpellCost = -12,
		int upgradeSpellCost = -10);
public:
	int getMoveCost(bool fastMode) const;

	int getAttack(bool longMode) const;

	int getDirectSpellCost() const;

	int getAreaSpellCost() const;

	int getTrapSpellCost() const;

	int getAllySpellCost() const;

	int getUpgradeSpellCost() const;
private:
	int getMoveSlowCost() const;

	int getMoveFastCost() const;
	
	int getAttackShortCost() const;

	int getAttackLongCost() const;
};