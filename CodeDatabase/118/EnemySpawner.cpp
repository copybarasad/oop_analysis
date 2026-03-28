#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(std::pair<int, int> pos, int coolDown, int radius)
	: Entity(SPAWNER_HEALTH, pos), coolDownSpawner(coolDown), spawnRadius(radius){}

EnemySpawner::EnemySpawner(int health, std::pair<int, int> pos, int coolDown, int radius)
	: Entity(health, pos), coolDownSpawner(coolDown), spawnRadius(radius) {}

int EnemySpawner::GetCoolDown() const {
	return coolDownSpawner;
}

int EnemySpawner::GetSpawnRadius() const {
	return spawnRadius;
}
