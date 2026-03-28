#include "EnemyTower.h"
#include "GameField.h"
#include "MathUtils.h"
#include <stdexcept>
#include <algorithm>

EnemyTower::EnemyTower(int x, int y, int attackRadius, int damage, int cooldownTurns)
	: positionX(x), positionY(y), attackRadius(attackRadius), damage(damage),
	  cooldownTurns(cooldownTurns), turnsUntilAttack(0) {
	validateInvariant();
}

int EnemyTower::GetX() const {
	return positionX;
}

int EnemyTower::GetY() const {
	return positionY;
}

int EnemyTower::GetAttackRadius() const {
	return attackRadius;
}

int EnemyTower::GetDamage() const {
	return damage;
}

int EnemyTower::GetCooldownTurns() const {
	return cooldownTurns;
}

int EnemyTower::GetCurrentCooldown() const {
	return turnsUntilAttack;
}

bool EnemyTower::CanAttack() const {
	return turnsUntilAttack <= 0;
}

void EnemyTower::Update(GameField& field) {
	if (turnsUntilAttack > 0) {
		turnsUntilAttack--;
	}
	
	if (CanAttack()) {
		Attack(field);
	}
}

void EnemyTower::Attack(GameField& field) {
	int playerX = field.GetPlayerX();
	int playerY = field.GetPlayerY();
	
	int distance = MathUtils::CalculateDistance(positionX, positionY, playerX, playerY);
	if (distance <= attackRadius && CanAttack()) {
		turnsUntilAttack = cooldownTurns;
	}
}

void EnemyTower::validateInvariant() const {
	if (attackRadius < 0) {
		throw std::runtime_error("Enemy tower attack radius cannot be negative");
	}
	if (damage < 0) {
		throw std::runtime_error("Enemy tower damage cannot be negative");
	}
	if (cooldownTurns < 0) {
		throw std::runtime_error("Enemy tower cooldown cannot be negative");
	}
	if (turnsUntilAttack < 0) {
		throw std::runtime_error("Enemy tower turns until attack cannot be negative");
	}
}
