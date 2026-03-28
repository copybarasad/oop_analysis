#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "game_field.h"
#include "enemy.h"
#include "ally.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "combat_system.h"
#include "enemy_manager.h"
#include "entity_manager.h"
#include "spell_manager.h"
#include "renderer.h"
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>

class GameInputHandler;
class GameInitializer;
class GameTurnProcessor;
class GameEndHandler;
class GameStateManager;
class GamePersistence;

class Game {
public:
    Player player;
    GameField field;
    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> buildings;
    std::vector<EnemyTower> towers;
    std::vector<Ally> allies;
    std::unique_ptr<CombatSystem> combatSystem;
    std::unique_ptr<EnemyManager> enemyManager;
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SpellManager> spellManager;
    std::unique_ptr<Renderer> renderer;

    bool gameOver;
    bool playerSkippingTurn;
    int playerTurnCounter;
    int currentLevel;
    int enhancementLevel;

    std::unique_ptr<GameInputHandler> inputHandler;
    std::unique_ptr<GameInitializer> initializer;
    std::unique_ptr<GameTurnProcessor> turnProcessor;
    std::unique_ptr<GameEndHandler> endHandler;
    std::unique_ptr<GameStateManager> stateManager;
    std::unique_ptr<GamePersistence> persistence;

public:
    Game(size_t fieldWidth = 15, size_t fieldHeight = 15);
    ~Game();

    void run();
    void initializeGame();
    void nextLevel();
    std::pair<int, int> generateRandomValidPosition() const;

    void processPlayerTurn(char command);
    void processEnemyTurns();
    void processAlliesTurn();
    void processBuildingTurns();
    void processTowerTurns();

    bool isGameOver() const;
    bool playerWon() const;
    int getPlayerScore() const;
    
    void displayField() const;
    void displayPlayerStatus() const;
    void displaySpells() const;
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    void saveToStream(std::ostream& out) const;
    void loadFromStream(std::istream& in);
    void setLastError(const std::string& e);
    std::string getLastError() const { return lastError; }
    
    void cleanupDestroyedBuildings();
    void cleanupDestroyedTowers();

private:
    std::string lastError;

    void cleanupDestroyedEntities(std::vector<EnemyBuilding>& buildings);
    void cleanupDestroyedEntities(std::vector<EnemyTower>& towers);
};

#endif