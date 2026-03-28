#include "Ally.h"
#include "GameField.h"
#include "Enemy.h"
#include "MathUtils.h"
#include <stdexcept>
#include <algorithm>

Ally::Ally(int x, int y, int health, int damage)
	: positionX(x), positionY(y), currentHealth(health), maximumHealth(health), baseDamage(damage) {
	validateInvariant();
}

int Ally::GetX() const {
	return positionX;
}

int Ally::GetY() const {
	return positionY;
}

int Ally::GetHealth() const {
	return currentHealth;
}

int Ally::GetDamage() const {
	return baseDamage;
}

bool Ally::IsAlive() const {
	return currentHealth > 0;
}

void Ally::TakeDamage(int damage) {
	if (damage < 0) {
		throw std::invalid_argument("Damage cannot be negative");
	}
	currentHealth -= damage;
	if (currentHealth < 0) {
		currentHealth = 0;
	}
	validateInvariant();
}

void Ally::Move(GameField& field) {
	int playerX = field.GetPlayerX();
	int playerY = field.GetPlayerY();
	
	int nearestEnemyX = -1;
	int nearestEnemyY = -1;
	int nearestDistance = 1000;
	
	for (size_t i = 0; i < field.GetEnemyCount(); ++i) {
		if (!field.IsEnemyAliveAt(i)) continue;
		
		int enemyX = field.GetEnemyX(i);
		int enemyY = field.GetEnemyY(i);
		int distance = MathUtils::CalculateDistance(positionX, positionY, enemyX, enemyY);
		
		if (distance < nearestDistance) {
			nearestDistance = distance;
			nearestEnemyX = enemyX;
			nearestEnemyY = enemyY;
		}
	}
	
	if (nearestEnemyX == -1) {
		return;
	}
	
	int deltaX = 0;
	int deltaY = 0;
	
	if (nearestEnemyX > positionX) deltaX = 1;
	else if (nearestEnemyX < positionX) deltaX = -1;
	
	if (nearestEnemyY > positionY) deltaY = 1;
	else if (nearestEnemyY < positionY) deltaY = -1;
	
	int newX = positionX + deltaX;
	int newY = positionY + deltaY;
	
	if (field.IsValidPosition(newX, newY) && field.IsCellPassable(newX, newY) &&
		!field.IsEnemyAt(newX, newY) && !field.IsAllyAt(newX, newY) &&
		!field.IsBuildingAt(newX, newY) && !field.IsTowerAt(newX, newY) &&
		!(newX == playerX && newY == playerY)) {
		positionX = newX;
		positionY = newY;
	}
}

bool Ally::Attack(GameField& field) {
	for (size_t i = 0; i < field.GetEnemyCount(); ++i) {
		if (!field.IsEnemyAliveAt(i)) continue;
		
		int enemyX = field.GetEnemyX(i);
		int enemyY = field.GetEnemyY(i);
		int distance = MathUtils::CalculateDistance(positionX, positionY, enemyX, enemyY);
		
		if (distance <= 1) {
			int damage = baseDamage;
			field.AttackEnemyByIndex(i, damage);
			return true;
		}
	}
	return false;
}

void Ally::validateInvariant() const {
	if (currentHealth < 0 || currentHealth > maximumHealth) {
		throw std::runtime_error("Ally health invariant violated");
	}
	if (maximumHealth <= 0) {
		throw std::runtime_error("Ally maximum health must be positive");
	}
	if (baseDamage < 0) {
		throw std::runtime_error("Ally damage cannot be negative");
	}
}
