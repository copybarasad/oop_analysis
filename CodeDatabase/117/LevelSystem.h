#ifndef LEVELSYSTEM_H
#define LEVELSYSTEM_H

#include "GameState.h"
#include "../Enums.h"

class LevelSystem{
private:
    int levelNum;
    std::string levelTitle;
public:
    LevelSystem();
    void save(std::string saveFileName, GameState gameState);
    void load(std::string saveFileName, GameState& gameState);
    void loadNewGame(GameState& gameState);
    int loadNextLevel(GameState& gameState, UpgradeType type);
    void restartLevel(GameState& gameState);
    int getLevelNum() const;
    void setLevelNum(int newLevelNum);
    std::string getLevelName() const;
    void loadSave(GameState& gameState, int slotNum);
    void saveToSlot(GameState& gameState, int slotNum);
    size_t getHash(GameState& gameState) const;
};

#endif