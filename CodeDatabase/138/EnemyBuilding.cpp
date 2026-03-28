#include "EnemyBuilding.h"
#include <stdexcept>

EnemyBuilding::EnemyBuilding(int spawnInterval, int x, int y, int health) 
	: spawnIntervalTurns(spawnInterval), turnsUntilNextSpawn(spawnInterval), 
	  positionX(x), positionY(y), currentHealth(health), maximumHealth(health) {
	validateInvariant();
}

int EnemyBuilding::GetX() const {
	return positionX;
}

int EnemyBuilding::GetY() const {
	return positionY;
}

bool EnemyBuilding::ShouldSpawnEnemy() const {
	return turnsUntilNextSpawn <= 0;
}

int EnemyBuilding::GetSpawnInterval() const {
	return spawnIntervalTurns;
}

int EnemyBuilding::GetCurrentCounter() const {
	return turnsUntilNextSpawn;
}

int EnemyBuilding::GetHealth() const {
	return currentHealth;
}

bool EnemyBuilding::IsAlive() const {
	return currentHealth > 0;
}

void EnemyBuilding::TakeDamage(int damage) {
	if (damage < 0) {
		throw std::invalid_argument("Damage cannot be negative");
	}
	currentHealth -= damage;
	if (currentHealth < 0) {
		currentHealth = 0;
	}
	validateInvariant();
}

void EnemyBuilding::Update() {
	if (turnsUntilNextSpawn > 0) {
		turnsUntilNextSpawn--;
	}
}

void EnemyBuilding::ResetSpawnCounter() {
	turnsUntilNextSpawn = spawnIntervalTurns;
}

void EnemyBuilding::validateInvariant() const {
	if (spawnIntervalTurns <= 0) {
		throw std::runtime_error("Enemy building spawn interval must be positive");
	}
	if (turnsUntilNextSpawn < 0) {
		throw std::runtime_error("Turns until spawn cannot be negative");
	}
	if (currentHealth < 0 || currentHealth > maximumHealth) {
		throw std::runtime_error("Building health invariant violated");
	}
	if (maximumHealth <= 0) {
		throw std::runtime_error("Building maximum health must be positive");
	}
}
