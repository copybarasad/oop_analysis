#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H
#include "GameConstants.h"

class EnemyBuilding {
private:
	int turnsToSpawn;
	int currentTurn;
	int spawnHealth;
	int spawnDamage;

public:
	EnemyBuilding(int spawnTurns = GameConstants::DEFAULT_SPAWN_TURNS,
		int enemyHealth = GameConstants::DEFAULT_SPAWN_HEALTH,
		int enemyDamage = GameConstants::DEFAULT_SPAWN_DAMAGE
	);

	bool shouldSpawnEnemy();
	void update();

	int getSpawnHealth() const;
	int getSpawnDamage() const;
	int getTurnsToSpawn() const;
	int getCurrentTurn() const;
	void setCurrentTurn(int turn);
};

#endif