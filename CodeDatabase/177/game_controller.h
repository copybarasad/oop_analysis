#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "player.h"
#include "game_field.h"
#include "game_enums.h"
#include "enemy_manager.h"
#include "building_manager.h"
#include "combat_manager.h"
#include "player_controller.h"
#include "game_initializer.h"
#include "turn_processor.h"
#include "tower.h"
#include <vector>
#include <memory> 

class GameController {
private:
	Player player;
	GameField field;
	std::unique_ptr<EnemyManager> enemyManager;
	std::unique_ptr<BuildingManager> buildingManager;
	std::unique_ptr<CombatManager> combatManager;
	std::unique_ptr<PlayerController> playerController;
	std::unique_ptr<GameInitializer> gameInitializer;
	std::unique_ptr<TurnProcessor> turnProcessor;
	bool gameRunning;
	std::vector<Tower> towers;
	int currentLevel;
	GameState gameState;
	static const int MAX_LEVELS = 5;
	
	void initializeLevel(int level);
	LevelConfig getLevelConfig(int level) const;
	void removeHalfOfSpells();
	void checkLevelCompletion();
	
public:
	/**
	 * Конструктор
	 * @param fieldWidth Ширина поля
	 * @param fieldHeight Высота поля
	 * @param autoInitialize Автоматическая инициализация уровня
	 */
	GameController(int fieldWidth = 15, int fieldHeight = 15, bool autoInitialize = true);

	bool isGameRunning() const;
	const Player& getPlayer() const;
	const GameField& getField() const;
	const std::vector<Enemy>& getEnemies() const;
	const std::vector<EnemyBuilding>& getBuildings() const;
	const std::vector<Tower>& getTowers() const;
	const std::vector<Position>& getActiveTraps() const;

	Player& getPlayer() { return player; }
	GameField& getField() { return field; }
	EnemyManager& getEnemyManager() { return *enemyManager; } 
	BuildingManager& getBuildingManager() { return *buildingManager; } 
	CombatManager& getCombatManager() { return *combatManager; } 
	std::vector<Tower>& getTowers() { return towers; }
	
	void movePlayer(Direction direction);
	void switchPlayerCombatMode();
	void castSpell(int spellIndex, const Position& target);
	void buySpell();
	
	int getCurrentLevel() const;
	void setCurrentLevel(int level);
	GameState getGameState() const;
	bool isLevelComplete() const;
	void startNextLevel();
	void restartGame();
	void printLevelInfo() const;
};

#endif