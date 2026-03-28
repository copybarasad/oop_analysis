#pragma once
#include <string>
#include <vector>
#include "LevelConfig.h"

class GameLevel {
public:
    enum class LevelType {
        FOREST,
        CASTLE, 
        DUNGEON,
        VOLCANO,
        BOSS_ARENA
    };

    GameLevel(LevelType type, int levelNumber);
    
    const LevelConfig& getConfig() const { return m_config; }
    std::string getName() const { return m_config.name; }
    
private:
    LevelConfig m_config;
    
    void setupLevel(int levelIndex);
};