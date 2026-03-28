#include "EnemyBuilding.h"
#include <algorithm>

EnemyBuilding::EnemyBuilding(int spawnTurns, int enemyHealth, int enemyDamage)
	: turnsToSpawn(spawnTurns), currentTurn(0), spawnHealth(enemyHealth), spawnDamage(enemyDamage) {
	if (turnsToSpawn <= 0) {
		turnsToSpawn = GameConstants::DEFAULT_SPAWN_TURNS;
	}
	if (spawnHealth < 0) {
		spawnHealth = GameConstants::DEFAULT_SPAWN_HEALTH;
	}
	if (spawnDamage < 0) {
		spawnDamage = GameConstants::DEFAULT_SPAWN_DAMAGE;
	}
}

bool EnemyBuilding::shouldSpawnEnemy() {
	return currentTurn >= turnsToSpawn;
}

void EnemyBuilding::update() {
	currentTurn++;
}

int EnemyBuilding::getSpawnHealth() const {
	return spawnHealth;
}

int EnemyBuilding::getSpawnDamage() const {
	return spawnDamage;
}

int EnemyBuilding::getTurnsToSpawn() const {
	return turnsToSpawn;
}

int EnemyBuilding::getCurrentTurn() const {
	return currentTurn;
}

void EnemyBuilding::setCurrentTurn(int turn) {
	currentTurn = std::max(0, turn);
}