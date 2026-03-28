#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Ally.h"
#include "EnemyTower.h"
#include "Spell.h"
#include "EnhancementSpell.h"
#include "GameException.h"
#include "EventManager.h"
#include "GameEvent.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>

enum class GameState {
    RUNNING,
    PLAYER_DIED,
    PLAYER_WON
};

class Game {
private:
    std::unique_ptr<GameField> field;
    std::unique_ptr<Player> player;
    std::vector<Enemy> enemies;
    std::vector<EnemyBuilding> buildings;
    std::vector<EnemyTower> towers;
    std::vector<Ally> allies;
    GameState currentState;
    int turnNumber;
    int playerStartX;
    int playerStartY;
    int allyCounter;
    int currentLevel;
    int lastLevelUpScore; // Последний счет, при котором был переход на уровень
    
    void initializeGame();
    void placePlayer();
    void placeInitialEnemies();
    void placeBuildings();
    void advanceToNextLevel();
    void processPlayerInput(char input);
    void processEnemyTurns();
    void processBuildingSpawns();
    void processAllyTurns();
    void processTowerAttacks();
    void checkTrapTriggers();
    void checkGameEndConditions();
    void handleSlowingCellEffect();
    bool isValidPlayerMove(int newX, int newY) const;
    bool handlePlayerAttack(char direction);  // direction: 'w'/'a'/'s'/'d' (внутренние команды)
    bool handlePlayerSpell(int spellIndex, int targetX, int targetY);
    void showUpgradeMenu();
    bool enemyKilledThisTurn;
    void publishScoreChange(int oldScore, int newScore);  // Вспомогательный метод для публикации событий изменения счета

public:
    Game(int fieldWidth = 15, int fieldHeight = 15);
    
    // Game control
    void run();
    void startGame();
    void startLevel();
    void processTurn(char playerInput);
    void processTurnWithAttackDirection(char playerInput, char attackDirection);
    bool handleGameOver();
    
    // Save/Load
    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
    
    // Getters
    GameState getGameState() const;
    int getTurnNumber() const;
    const Player& getPlayer() const;
    Player& getPlayer();
    const GameField& getField() const;
    GameField& getField();
    const std::vector<Enemy>& getEnemies() const;
    std::vector<Enemy>& getEnemies();
    const std::vector<Ally>& getAllies() const;
    std::vector<Ally>& getAllies();
    const std::vector<EnemyBuilding>& getBuildings() const;
    std::vector<EnemyBuilding>& getBuildings();
    
    // Spell and game mechanics
    bool placeTrap(int x, int y, int damage);
    bool summonAlly(int x, int y);
    void onEnemyKilled();
    
    // Game information
    void displayGameStatus() const;
    void displayControls() const;
    bool isGameOver() const;
};

#endif // GAME_H

