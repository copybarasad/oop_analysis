#include "EnemyTower.h"
#include <cmath>

EnemyTower::EnemyTower(int range, int dmg)
	: attackRange(range), damage(dmg), canAttack(true), cooldown(2), currentCooldown(0) {}

bool EnemyTower::canAttackPlayer(int towerX, int towerY, int playerX, int playerY) const {
	if (!isReadyToAttack()) {
		return false;
	}

	int dx = std::abs(towerX - playerX);
	int dy = std::abs(towerY - playerY);
	return dx <= attackRange && dy <= attackRange;
}

void EnemyTower::attack() {
	canAttack = false;
	currentCooldown = cooldown;
}

void EnemyTower::update() {
	if (!canAttack) {
		currentCooldown--;
		if (currentCooldown <= 0) {
			canAttack = true;
		}
	}
}

int EnemyTower::getDamage() const {
	return damage;
}

int EnemyTower::getRange() const {
	return attackRange;
}

int EnemyTower::getCooldown() const {
	return cooldown;
}

int EnemyTower::getCurrentCooldown() const {
	return currentCooldown;
}

bool EnemyTower::isReadyToAttack() const {
	return canAttack;
}

bool EnemyTower::getCanAttackFlag() const {
	return canAttack;
}

void EnemyTower::resetCooldown() {
	currentCooldown = 0;
	canAttack = true;
}

void EnemyTower::setState(bool readyToAttack, int cooldownValue, int currentCooldownValue) {
	canAttack = readyToAttack;
	cooldown = cooldownValue;
	currentCooldown = currentCooldownValue;
	if (cooldown < 0) {
		cooldown = 0;
	}
	if (currentCooldown < 0) {
		currentCooldown = 0;
	}
	if (canAttack && currentCooldown > 0) {
		currentCooldown = 0;
	}
}