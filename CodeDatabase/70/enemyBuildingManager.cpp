#include "enemyBuildingManager.h"
void EnemyBuildingManager::addBuilding(int range, int reward, int health, int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	buildings[buildingPos] = EnemyBuilding(range, reward, health);
}
void EnemyBuildingManager::removeBuilding(int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	buildings.erase(buildingPos);
}
int EnemyBuildingManager::getReward(int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	return (it->second).getReward();
}

int EnemyBuildingManager::getBuildingRange(int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	return (it->second).getRange();
}

int EnemyBuildingManager::getBuildingHealth(int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	return (it->second).getHealth();
}

void EnemyBuildingManager::addBuildingSpell(int x, int y, Spell* spell)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	(it->second).addSpell(spell);
}

Spell* EnemyBuildingManager::getBuildingSpell(int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	return (it->second).useSpell();
}
void EnemyBuildingManager::receiveDamage(int x, int y, int damage)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	(it->second).receiveDamage(damage);
}
bool EnemyBuildingManager::empty()
{
	return buildings.empty();
}
bool EnemyBuildingManager::isBuildingAlive(int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	return (it->second).getHealth() > 0;
}
bool EnemyBuildingManager::isBuildingInPos(int x, int y) const
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	return !(it == buildings.end());
}
void EnemyBuildingManager::showBuildings() const
{
	for (const auto& [buildingPos, building] : buildings)
	{
		std::cout << std::format("Cell: {} {}: ", buildingPos.first, buildingPos.second);
		building.showStats();
	}
}

Spell* EnemyBuildingManager::getSpell(int x, int y)
{
	std::pair<int, int> buildingPos = {x, y};
	auto it = buildings.find(buildingPos);
	return (it->second).useSpell();
}
std::map<std::pair<int, int>, EnemyBuilding>& EnemyBuildingManager::getBuildings()
{
	return buildings;
}

const std::map<std::pair<int, int>, EnemyBuilding>& EnemyBuildingManager::getBuildings() const
{
	return buildings;
}

int EnemyBuildingManager::attackBuilding(int x, int y, int damage)
{
	int reward = 0;
	receiveDamage(x, y, damage);
	std::cout << std::format("Building at pos ({};{}) attacked! -{} health points!", x, y, damage) << std::endl;
	if (!isBuildingAlive(x, y))
	{
		reward = getReward(x, y);
		removeBuilding(x, y);
	}
	return reward;
}

void EnemyBuildingManager::setBuildings(std::map<std::pair<int, int>, EnemyBuilding> buildings)
{
	this->buildings = buildings;
}

size_t EnemyBuildingManager::getBuildingsNum() const
{
	return buildings.size();
}

std::ostream& operator << (std::ostream &os, const std::pair<int, int> &pair);
std::istream& operator >> (std::istream& in, std::pair<int, int>& pair);

std::istream& operator >> (std::istream &in, EnemyBuilding& enemyBuilding);
std::ostream& operator << (std::ostream &os, const EnemyBuilding &enemyBuilding);

std::ostream& operator << (std::ostream &os, const EnemyBuildingManager &enemyBuildingManager)
{
	os << enemyBuildingManager.getBuildingsNum() << " ";
	std::map<std::pair<int, int>, EnemyBuilding> buildings = enemyBuildingManager.getBuildings();
	for (const auto& [enemyPos, enemy] : buildings)
	{
		os << enemyPos << " " << enemy;
	}
	return os;
}
std::istream& operator >> (std::istream& in, EnemyBuildingManager& enemyBuildingManager)
{
	std::map<std::pair<int, int>, EnemyBuilding> buildings;
	size_t enemiesNum;
	in >> enemiesNum;
	if (!in.good()) throw std::runtime_error("Failed to read num of buildings");
	std::pair<int, int> enemyPos;
	EnemyBuilding enemyBuilding;
	for(size_t i = 0; i < enemiesNum; ++i)
	{
		in >> enemyPos >> enemyBuilding;
		if (!in.good()) throw std::runtime_error("Failed to read key-value in enemy building manager");
		buildings[enemyPos] = enemyBuilding;
	}
	enemyBuildingManager.setBuildings(buildings);
	return in;
}