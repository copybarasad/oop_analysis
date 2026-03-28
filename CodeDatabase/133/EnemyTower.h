#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "GameConstants.h"

class EnemyTower {
private:
	int attackRange;
	int damage;
	bool canAttack;
	int cooldown;
	int currentCooldown;

public:
	EnemyTower(int range = 3, int dmg = 8);

	bool canAttackPlayer(int towerX, int towerY, int playerX, int playerY) const;
	void attack();
	void update();
	int getDamage() const;
	int getRange() const;
	int getCooldown() const;
	int getCurrentCooldown() const;
	bool isReadyToAttack() const;
	bool getCanAttackFlag() const;
	void resetCooldown();
	void setState(bool readyToAttack, int cooldownValue, int currentCooldownValue);
};

#endif