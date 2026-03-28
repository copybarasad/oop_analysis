#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <vector>
#include <memory>

struct GameSaveData;

class GameController {
private:
    GameField* gameField;
    Player player;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers;
    int currentTurn;
    bool gameRunning;

public:
    GameController(GameField* field);
    
    void initializeGame();
    void startGameLoop();
    void displayGameState() const;
    bool isGameOver() const;
    void processPlayerTurn(char input);
    void processEnemyTurn();
    void processTowerTurn();
    void checkWinCondition();
    void displayFinalStatistics() const;
    void loadFromSave(const GameSaveData& saveData);

private:
    void createEnemies();
    void createTowers();
    void giveAllSpells();
    void handleSpellCasting();
    void handleAutoAttack();
    void saveGame();
    void loadGame();
    void showSaveLoadMenu();
    std::string getSaveSlotName();
};

#endif