#ifndef LEVELCONFIG_H
#define LEVELCONFIG_H

#include "GameMode.h"

struct LevelConfig {
    int levelNumber;
    int fieldSize;
    int enemyCount;
    int enemyHealth;
    int enemyDamage;
    int buildingCount;
    int towerCount;
    int victoryScore;
    
    LevelConfig(int level = 1, GameMode mode = GameMode::CUSTOM) 
        : levelNumber(level),
          fieldSize(calculateFieldSize(level, mode)),
          enemyCount(calculateEnemyCount(level, mode)),
          enemyHealth(calculateEnemyHealth(level, mode)),
          enemyDamage(calculateEnemyDamage(level, mode)),
          buildingCount(level <= 5 ? 1 : 2),
          towerCount(level <= 5 ? 1 : 2),
          victoryScore(calculateVictoryScore(level, mode)) {}
    
private:
    static int calculateFieldSize(int level, GameMode mode) {
        if (mode == GameMode::LEVELS) {
            switch (level) {
                case 1: return 10;
                case 2: return 15;
                case 3: return 20;
                default: return 10;
            }
        }
        return 15 + level * 3;
    }
    
    static int calculateVictoryScore(int level, GameMode mode) {
        if (mode == GameMode::LEVELS) {
            switch (level) {
                case 1: return 100;
                case 2: return 150;
                case 3: return 200;
                default: return 100;
            }
        }
        return 1200;
    }
    
    static int calculateEnemyCount(int level, GameMode mode) {
        if (mode == GameMode::LEVELS) {
            switch (level) {
                case 1: return 3;
                case 2: return 5;
                case 3: return 7;
                default: return 3;
            }
        }
        return 3 + level;
    }
    
    static int calculateEnemyHealth(int level, GameMode mode) {
        if (mode == GameMode::LEVELS) {
            switch (level) {
                case 1: return 40;
                case 2: return 60;
                case 3: return 80;
                default: return 40;
            }
        }
        return 40;
    }
    
    static int calculateEnemyDamage(int level, GameMode mode) {
        if (mode == GameMode::LEVELS) {
            switch (level) {
                case 1: return 10;
                case 2: return 20;
                case 3: return 30;
                default: return 10;
            }
        }
        return 10;
    }
};

#endif
