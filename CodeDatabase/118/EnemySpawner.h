#ifndef EnemySpawner_H
#define EnemySpawner_H

#include "Entity.h"
#include "Constants.h"

class EnemySpawner : public Entity
{
	int coolDownSpawner, spawnRadius;
public:
	EnemySpawner(std::pair<int,int> pos = std::pair<int, int>(10, 10), int = SPAWNER_COOLDOWN, int = SPAWNER_RADIUS);
	EnemySpawner(int, std::pair<int, int>, int, int);
	int GetCoolDown()const;
	int GetSpawnRadius()const;
};

#endif