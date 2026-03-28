#pragma once

#include "Enemy.hpp"

class EnemyBuilding {
public:
	EnemyBuilding(int hp = 100, int spawnInterval = 5, int maxSpawnCount = -1);
	EnemyBuilding(const EnemyBuilding& other);
	EnemyBuilding(EnemyBuilding&& other) noexcept;
	EnemyBuilding& operator=(const EnemyBuilding& other);
	EnemyBuilding& operator=(EnemyBuilding&& other) noexcept;
	~EnemyBuilding() = default;

	bool update();
	void reset();
	int getSpawnInterval() const noexcept;
	int getTurnsUntilSpawn() const noexcept;
	bool canSpawn() const noexcept;
	int getSpawnCount() const noexcept;
	int getMaxSpawnCount() const noexcept;
	
	void takeDamage(int amount);
	bool isAlive() const noexcept;
	int getHP() const noexcept;
	
	void setHP(int hp);
	void setTurnsUntilSpawn(int turns);
	void setSpawnCount(int count);

private:
	int hp;
	int spawnInterval;
	int turnsUntilSpawn;
	int spawnCount;
	int maxSpawnCount;
};
