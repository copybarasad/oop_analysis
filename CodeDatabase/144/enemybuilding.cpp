#include "enemybuilding.h"
#include <algorithm>

EnemyBuilding::EnemyBuilding(int spawnInterval, int x, int y, int health) 
	: spawnInterval(spawnInterval), turnsUntilSpawn(0), x(x), y(y), 
	  health(health), maxHealth(health) {}

void EnemyBuilding::update() {
	if (turnsUntilSpawn > 0) {
		turnsUntilSpawn--;
	}
}

bool EnemyBuilding::shouldSpawnEnemy() {
	return turnsUntilSpawn <= 0 && !isDestroyed();
}

void EnemyBuilding::resetSpawnTimer() {
	turnsUntilSpawn = spawnInterval;
}

int EnemyBuilding::getTurnsUntilSpawn() const {
	return turnsUntilSpawn;
}

int EnemyBuilding::getSpawnInterval() const {
	return spawnInterval;
}

int EnemyBuilding::getX() const {
	return x;
}

int EnemyBuilding::getY() const {
	return y;
}

int EnemyBuilding::getPositionX() const {
	return x;
}

int EnemyBuilding::getPositionY() const {
	return y;
}

void EnemyBuilding::setPosition(int newX, int newY) {
	x = newX;
	y = newY;
}

void EnemyBuilding::setSpawnInterval(int value) {
	spawnInterval = value;
}

void EnemyBuilding::setTurnsUntilSpawn(int value) {
	turnsUntilSpawn = value;
}

int EnemyBuilding::getHealth() const {
	return health;
}

int EnemyBuilding::getMaxHealth() const {
	return maxHealth;
}

void EnemyBuilding::takeDamage(int damageAmount) {
	health = std::max(0, health - damageAmount);
}

bool EnemyBuilding::isDestroyed() const {
	return health <= 0;
}

bool EnemyBuilding::isAlive() const {
	return !isDestroyed();
}

void EnemyBuilding::setHealth(int value) {
	health = value;
}

void EnemyBuilding::setMaxHealth(int value) {
	maxHealth = value;
}

EnemyBuilding::Snapshot EnemyBuilding::createSnapshot() const {
	Snapshot snapshot;
	snapshot.spawnInterval = spawnInterval;
	snapshot.turnsUntilSpawn = turnsUntilSpawn;
	snapshot.positionX = x;
	snapshot.positionY = y;
	snapshot.health = health;
	snapshot.maxHealth = maxHealth;
	return snapshot;
}

EnemyBuilding EnemyBuilding::fromSnapshot(const Snapshot& snapshot) {
	EnemyBuilding building(snapshot.spawnInterval, snapshot.positionX, snapshot.positionY, snapshot.maxHealth);
	building.setTurnsUntilSpawn(snapshot.turnsUntilSpawn);
	building.setHealth(snapshot.health);
	building.setMaxHealth(snapshot.maxHealth);
	return building;
}