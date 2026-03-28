#pragma once
#include "Entity.h"
#include "struct/dataBuilding.h"

class EnemyBuilding : public Entity {
private:
	int spawnCooldown;
	int spawnInterval;
public:
	EnemyBuilding(int interval);
	EnemyBuilding(dataBuilding data);
	~EnemyBuilding() override;
	bool Update();
	int GetSpawnCooldown() const;
	int GetSpawnInterval() const;
	dataBuilding Save();
};
