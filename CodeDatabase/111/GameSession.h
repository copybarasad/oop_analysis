#pragma once
#include "GameBoard.h"
#include "Hero.h"
#include "Foe.h"
#include "EnemyTower.h"
#include "SpellShop.h"
#include "TurnManager.h"
#include "GameSaveManager.h"
#include "LevelManager.h"
#include <vector>
#include <memory>

class GameSession {
private:
    GameBoard board;
    Hero playerHero;
    std::vector<Foe> enemies;
    std::vector<EnemyTower> towers;
    
    SpellShop spellShop;
    TurnManager turnManager;
    GameSaveManager saveManager;
    LevelManager levelManager;

    bool gameRunning;
    bool gameLoaded;

public:
    GameSession();
    ~GameSession() = default;

    void saveGame(const std::string& filename);
    bool loadGame(const std::string& filename);
    void startNewGame();
    void openShop() { spellShop.openShop(); }
    
    bool isPlayerTurn() const { return turnManager.isPlayerTurn(); }
    void endPlayerTurn() { turnManager.switchTurn(); }
    void processEnemyTurn();
    void checkCollisions();
    
    void clearGame() {
        enemies.clear();
        towers.clear();
        board.removeAllActors();
    }

    const GameSaveManager& getSaveManager() const { return saveManager; }
    GameSaveManager& getSaveManager() { return saveManager; }
    
    const GameBoard& getBoard() const { return board; }
    GameBoard& getBoard() { return board; }
    
    const Hero& getHero() const { return playerHero; }
    Hero& getHero() { return playerHero; }
    
    const std::vector<Foe>& getEnemies() const { return enemies; }
    const std::vector<EnemyTower>& getTowers() const { return towers; }
    
    bool isGameOver() const;
    bool isVictory() const;
    bool isDefeat() const;
    
private:
    void initializeGame();
    void cleanupDestroyedActors();
};