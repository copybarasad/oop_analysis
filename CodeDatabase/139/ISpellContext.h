#pragma once
#include "DataTypes/Point.h"
#include "Entities/Enemy.h"
#include "Entities/Tower.h"
#include <vector>


class ISpellContext
{
public:
	virtual std::vector<Enemy*> getEnemiesAt(const Point& point, int radius) = 0;
	virtual void spawnAlly(int health, int damage, int alliesCount, const Point& point) = 0;
	virtual Entity* getEntityAt(const Point& point) = 0;
	virtual Enemy* getEnemyAt(const Point& point) = 0;
	virtual Tower* getTowerAt(const Point& point, int radius) = 0;
	virtual Tower* getTowerAt(const Point& point) = 0;
	virtual bool spawnTrap(const Point& point) = 0;
	virtual void upgradeSpell(int count) = 0;
};