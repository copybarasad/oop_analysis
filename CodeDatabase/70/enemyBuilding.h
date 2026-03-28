#include "spell.h"
#include "spellsFactory.h"
#include <iostream>
#include <format>

#ifndef ENEMYBUILDING_H
#define ENEMYBUILDING_H

class EnemyBuilding
{
private:
	Spell* spell;
	int range;
	int reward;
	int health;
public:
	~EnemyBuilding();
	EnemyBuilding();
	EnemyBuilding(int range, int reward, int health);
	EnemyBuilding(const EnemyBuilding& other);
	EnemyBuilding& operator=(const EnemyBuilding& other);
	void addSpell(Spell* spell);
	Spell* getSpell() const;
	Spell* useSpell();
	void receiveDamage(int damageValue);
	int getRange() const;
	int getReward() const;
	int getHealth() const;

	void setRange(int value);
	void setReward(int value);
	void setHealth(int value);

	void showStats() const;
};

#endif
