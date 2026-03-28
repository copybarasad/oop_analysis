#ifndef GAME_SAVE_H
#define GAME_SAVE_H

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <utility>
#include "GameSaveExceptions.h"

class GameController;

struct GameSaveData {
    int currentLevel;
    int playerScore;
    int playerHealth;
    int playerMana;
    int enemiesKilled;
    int playerX;
    int playerY;
    bool hasStartingSpell;
    int playerCombatMode;
    
    int fieldWidth;
    int fieldHeight;
    std::vector<int> cellTypes;
    std::vector<std::pair<int, int>> enemyPositions;
    std::vector<std::pair<int, int>> trapPositions;
    std::vector<std::pair<int, int>> allyPositions;
    
    GameSaveData();
    bool isValid() const;
};

class GameSave {
private:
    std::string saveFileName;
    
    void saveToFile(const GameSaveData& data) const;
    std::unique_ptr<GameSaveData> loadFromFile() const;
    void validateSaveData(const GameSaveData& data) const;

public:
    GameSave(const std::string& fileName = "game_save.dat");
    
    bool saveGame(const GameController& controller);
    std::unique_ptr<GameSaveData> loadGame();
    bool saveExists() const;
};

#endif
