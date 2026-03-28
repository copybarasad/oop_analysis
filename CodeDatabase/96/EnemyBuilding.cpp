#include "EnemyBuilding.h"

EnemyBuilding::EnemyBuilding(int interval)
	: Entity(200, 0), spawnCooldown(interval), spawnInterval(interval) {}

EnemyBuilding::~EnemyBuilding() = default;

EnemyBuilding::EnemyBuilding(dataBuilding data) : Entity(data.health, data.damage), spawnCooldown(data.spawnCooldown), spawnInterval(data.spawnInterval){
	SetStunDuration(data.stunDuration);

}

bool EnemyBuilding::Update() {
	if (spawnCooldown > 0) {
		spawnCooldown--;
		return false;
	} else {
		spawnCooldown = spawnInterval;
		return true;
	}
}

int EnemyBuilding::GetSpawnCooldown() const {
	return spawnCooldown;
}

int EnemyBuilding::GetSpawnInterval() const {
	return spawnInterval;
}

dataBuilding EnemyBuilding::Save(){
	dataBuilding data;
	data.spawnCooldown=spawnCooldown;
	data.spawnInterval=spawnInterval;
	data.damage=damage;
	data.health=health;
	data.stunDuration=stunDuration;
	return data;
}