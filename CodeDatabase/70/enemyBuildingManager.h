#include <utility>
#include <map>
#include "spell.h"
#include "enemyBuilding.h"
#include "spellsFactory.h"
#include <format>
#include <iostream>

#ifndef ENEMYBUILDINGMANAGER_H
#define ENEMYBUILDINGMANAGER_H

class EnemyBuildingManager
{
private:
	std::map<std::pair<int, int>, EnemyBuilding> buildings;
public:
	void addBuilding(int range, int reward, int health, int x, int y);
	void removeBuilding(int x, int y);
	int getReward(int x, int y);
	int getBuildingRange(int x, int y);
	int getBuildingHealth(int x, int y);
	void addBuildingSpell(int x, int y, Spell* spell);
	Spell* getBuildingSpell(int x, int y);
	void receiveDamage(int x, int y, int damage);
	bool empty();
	bool isBuildingAlive(int x, int y);
	bool isBuildingInPos(int x, int y) const;
	void showBuildings() const;
	std::map<std::pair<int, int>, EnemyBuilding>& getBuildings();
	const std::map<std::pair<int, int>, EnemyBuilding>& getBuildings() const;
	void setBuildings(std::map<std::pair<int, int>, EnemyBuilding> buildings);
	size_t getBuildingsNum() const;
	int attackBuilding(int x, int y, int damage);
	Spell* getSpell(int x, int y);
};

#endif