#pragma once

#include <memory>
#include <vector>
#include "Field.h"
#include "Player.h"
#include "Enemy.h"
#include "Hand.h"
#include "SaveGame.h"

class Game {
private:
    bool gameRunning;
    int turnCount;
    int currentLevel;

    std::unique_ptr<Field> field;
    std::unique_ptr<Player> player;
    std::unique_ptr<Hand> hand;
    std::vector<Enemy> enemies;

public:
    Game();
    void startGame();

    void initializeGame();
    void nextLevel();
    void initializeFromSave(const SaveGame& saveData);
    SaveGame createSaveData() const;

    void saveGame();
    void loadGame();

    void showLevelCompleteMenu();
    void showGameOverMenu();

    void processEnemiesTurn();
    void processTowersTurn();

    bool checkWinCondition() const;
    bool checkLoseCondition() const;

    void movePlayer(char direction);
    void attackEnemies();
    void attackTower();
    void switchPlayerMode();
    void useSpell(int spellIndex);
    void showHelp() const;
    void stop() { gameRunning = false; }
    bool isRunning() const { return gameRunning; }

    void render() const;

    const Field& getField() const { return *field; }
    Field& getField() { return *field; }
    const Player& getPlayer() const { return *player; }
    Player& getPlayer() { return *player; }
};