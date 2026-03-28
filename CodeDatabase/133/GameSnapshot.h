#ifndef GAME_SNAPSHOT_H
#define GAME_SNAPSHOT_H

#include <vector>
#include "Cell.h"
#include "Player.h"
#include "Position.h"

struct EnemySnapshot {
	Position position;
	int health;
	int damage;
};

struct BuildingSnapshot {
	Position position;
	int turnsToSpawn;
	int currentTurn;
	int spawnHealth;
	int spawnDamage;
};

struct TowerSnapshot {
	Position position;
	int range;
	int damage;
	bool canAttack;
	int cooldown;
	int currentCooldown;
};

struct AllySnapshot {
	Position position;
	int health;
	int damage;
	int lifetime;
};

struct TrapSnapshot {
	Position position;
	int damage;
};

struct GameFieldSnapshot {
	int width;
	int height;
	Position playerPosition;
	PlayerState playerState;
	std::vector<std::vector<CellType>> cellTypes;
	std::vector<EnemySnapshot> enemies;
	std::vector<BuildingSnapshot> buildings;
	std::vector<TowerSnapshot> towers;
	std::vector<AllySnapshot> allies;
	std::vector<TrapSnapshot> traps;
};

struct GameSnapshot {
	int level;
	GameFieldSnapshot field;
};

#endif

