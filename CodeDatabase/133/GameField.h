#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include <memory>
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "GameConstants.h"
#include "Position.h"
#include "GameSnapshot.h"

class GameField {
private:
	int width;
	int height;
	std::vector<std::vector<Cell>> grid;
	Player player;
	Position playerPosition;

	std::vector<std::pair<Position, Enemy>> enemies;
	std::vector<std::pair<Position, EnemyBuilding>> buildings;
	std::vector<std::pair<Position, EnemyTower>> towers;
	std::vector<std::pair<Position, Ally>> allies;
	std::vector<std::pair<Position, int>> traps;

	void generateImpassableCells();
	void generateSlowingCells();
	void resolveTrapsInternal();
	void updateAlliesInternal();
	void updateTowersInternal();
	void rebuildOccupancy();

public:
	GameField(int fieldWidth, int fieldHeight,
		int playerHealth = GameConstants::DEFAULT_PLAYER_HEALTH,
		int playerMeleeDamage = GameConstants::DEFAULT_PLAYER_MELEE_DAMAGE,
		int playerRangedDamage = GameConstants::DEFAULT_PLAYER_RANGED_DAMAGE);

	bool isValidPosition(int x, int y) const;

	std::vector<std::pair<Position, Enemy>>& getEnemies() { return enemies; }
	std::vector<std::pair<Position, EnemyBuilding>>& getBuildings() { return buildings; }
	std::vector<std::vector<Cell>>& getGrid() { return grid; }
	const std::vector<std::vector<Cell>>& getGrid() const { return grid; }
	Player& getPlayer() { return player; }
	const Player& getPlayer() const { return player; }

	int getWidth() const { return width; }
	int getHeight() const { return height; }
	Position getPlayerPosition() const { return playerPosition; }

	bool movePlayer(int dx, int dy);
	bool attackEnemy(int dx, int dy);
	bool castSpell(int spellIndex, int targetX, int targetY);
	void switchPlayerAttackType();
	void updateEnemies();
	void updateBuildings();
	void performEnemyTurn();
	void performAllyTurn();
	void performBuildingTurn();
	void performTowerTurn();
	void resolveTraps();
	void updateGameState();
	bool hasHostileUnits() const;
	Position findRandomFreeCell() const;

	void addEnemy(int x, int y,
		int health = GameConstants::DEFAULT_ENEMY_HEALTH,
		int damage = GameConstants::DEFAULT_ENEMY_DAMAGE);
	void addBuilding(int x, int y,
		int spawnTurns = GameConstants::DEFAULT_SPAWN_TURNS,
		int enemyHealth = GameConstants::DEFAULT_SPAWN_HEALTH,
		int enemyDamage = GameConstants::DEFAULT_SPAWN_DAMAGE);
	void addTower(int x, int y, int range = 3, int damage = 8);
	void addAlly(int x, int y, int health, int damage);
	void addTrap(int x, int y, int damage);

	void accumulateEnhancement() { player.accumulateEnhancement(); }

	void display() const;
	void displaySpellShop();

	GameFieldSnapshot captureSnapshot() const;
	void restoreFromSnapshot(const GameFieldSnapshot& snapshot);

	// Запрет копирования
	GameField(const GameField&) = delete;
	GameField& operator=(const GameField&) = delete;

	// Разрешение перемещения
	GameField(GameField&&) = default;
	GameField& operator=(GameField&&) = default;

	~GameField() = default;
};

#endif