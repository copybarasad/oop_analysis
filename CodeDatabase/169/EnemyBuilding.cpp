#include "EnemyBuilding.hpp"

EnemyBuilding::EnemyBuilding(int hp, int spawnInterval, int maxSpawnCount)
	: hp(hp),
	  spawnInterval(spawnInterval),
	  turnsUntilSpawn(spawnInterval),
	  spawnCount(0),
	  maxSpawnCount(maxSpawnCount) {}

EnemyBuilding::EnemyBuilding(const EnemyBuilding& other)
	: hp(other.hp),
	  spawnInterval(other.spawnInterval),
	  turnsUntilSpawn(other.turnsUntilSpawn),
	  spawnCount(other.spawnCount),
	  maxSpawnCount(other.maxSpawnCount) {}

EnemyBuilding::EnemyBuilding(EnemyBuilding&& other) noexcept
	: hp(other.hp),
	  spawnInterval(other.spawnInterval),
	  turnsUntilSpawn(other.turnsUntilSpawn),
	  spawnCount(other.spawnCount),
	  maxSpawnCount(other.maxSpawnCount) {
	other.hp = 0;
	other.spawnInterval = 5;
	other.turnsUntilSpawn = 5;
	other.spawnCount = 0;
	other.maxSpawnCount = 3;
}

EnemyBuilding& EnemyBuilding::operator=(const EnemyBuilding& other) {
	if (this != &other) {
		hp = other.hp;
		spawnInterval = other.spawnInterval;
		turnsUntilSpawn = other.turnsUntilSpawn;
		spawnCount = other.spawnCount;
		maxSpawnCount = other.maxSpawnCount;
	}
	return *this;
}

EnemyBuilding& EnemyBuilding::operator=(EnemyBuilding&& other) noexcept {
	if (this != &other) {
		hp = other.hp;
		spawnInterval = other.spawnInterval;
		turnsUntilSpawn = other.turnsUntilSpawn;
		spawnCount = other.spawnCount;
		maxSpawnCount = other.maxSpawnCount;
		
		other.hp = 0;
		other.spawnInterval = 5;
		other.turnsUntilSpawn = 5;
		other.spawnCount = 0;
		other.maxSpawnCount = 3;
	}
	return *this;
}

bool EnemyBuilding::update() {
	if (maxSpawnCount >= 0 && spawnCount >= maxSpawnCount) {
		return false;
	}
	
	turnsUntilSpawn--;
	if (turnsUntilSpawn <= 0) {
		turnsUntilSpawn = spawnInterval;
		spawnCount++;
		return true;
	}
	
	return false;
}

void EnemyBuilding::reset() {
	turnsUntilSpawn = spawnInterval;
}

int EnemyBuilding::getSpawnInterval() const noexcept {
	return spawnInterval; 
}

int EnemyBuilding::getTurnsUntilSpawn() const noexcept {
	return turnsUntilSpawn; 
}

bool EnemyBuilding::canSpawn() const noexcept {
	return maxSpawnCount < 0 || spawnCount < maxSpawnCount;
}

int EnemyBuilding::getSpawnCount() const noexcept {
	return spawnCount; 

}

int EnemyBuilding::getMaxSpawnCount() const noexcept {
	return maxSpawnCount; 
}

void EnemyBuilding::takeDamage(int amount) {
	if (amount <= 0) return;
	if (amount >= hp) hp = 0;
	else hp -= amount;
}

bool EnemyBuilding::isAlive() const noexcept {
	return hp > 0;
}

int EnemyBuilding::getHP() const noexcept {
	return hp;
}

void EnemyBuilding::setHP(int hp) {
	if (hp < 0) this->hp = 0;
	else this->hp = hp;
}

void EnemyBuilding::setTurnsUntilSpawn(int turns) {
	if (turns < 0) turnsUntilSpawn = 0;
	else turnsUntilSpawn = turns;
}

void EnemyBuilding::setSpawnCount(int count) {
	if (count < 0) spawnCount = 0;
	else spawnCount = count;
}
