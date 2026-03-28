#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "GameField.h"
#include "Player.h"
#include "Level.h"
#include "SaveSystem.h"
#include <memory>

class GameSession {
private:
    std::unique_ptr<GameField> gameField;
    std::unique_ptr<Player> player;
    std::unique_ptr<Level> currentLevel;
    int currentLevelNumber;
    bool sessionActive;

public:
    GameSession();
    ~GameSession();
    
    void startNewGame();
    void loadSavedGame(const std::string& saveName);
    void startLevel(int levelNumber);
    void processGameLoop();
    void endGame();
    void displaySessionStatus() const;
    
    bool isActive() const;
    int getCurrentLevel() const;

private:
    void startLoadedLevel(int levelNumber, const GameSaveData& saveData);
    void setupGameField();
    void setupPlayer();
    void cleanup();
};

#endif