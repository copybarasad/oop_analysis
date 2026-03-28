#include "Enemy.h"
#include <stdexcept>

Enemy::Enemy(int health, int damage) 
	: currentHealth(health), maximumHealth(health), baseDamage(damage), 
	  positionX(0), positionY(0) {
	validateInvariant();
}

int Enemy::GetHealth() const {
	return currentHealth;
}

int Enemy::GetMaxHealth() const {
	return maximumHealth;
}

bool Enemy::IsAlive() const {
	return currentHealth > 0;
}

void Enemy::TakeDamage(int damage) {
	if (damage < 0) {
		throw std::invalid_argument("Damage cannot be negative");
	}
	currentHealth -= damage;
	if (currentHealth < 0) {
		currentHealth = 0;
	}
	validateInvariant();
}

int Enemy::GetX() const {
	return positionX;
}

int Enemy::GetY() const {
	return positionY;
}

void Enemy::SetPosition(int x, int y) {
	positionX = x;
	positionY = y;
}


int Enemy::GetCurrentDamage() const {
	return baseDamage;
}

int Enemy::GetBaseDamage() const {
	return baseDamage;
}

void Enemy::validateInvariant() const {
	if (currentHealth < 0 || currentHealth > maximumHealth) {
		throw std::runtime_error("Enemy health invariant violated");
	}
	if (maximumHealth <= 0) {
		throw std::runtime_error("Enemy maximum health must be positive");
	}
	if (baseDamage < 0) {
		throw std::runtime_error("Enemy damage cannot be negative");
	}
}
