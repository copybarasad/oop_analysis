#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include "GameController.h"
#include <random>
#include <cstdint>

class LevelGenerator {
private:
    uint32_t seed;
    std::mt19937 rng;
    
public:
    LevelGenerator(uint32_t levelSeed = 0) : seed(levelSeed), rng(levelSeed) {}
    
    void generateLevel(GameController& game, int levelNumber);
    void setSeed(uint32_t newSeed) { 
        seed = newSeed; 
        rng.seed(seed);
    }
    uint32_t getSeed() const { return seed; }
    
    static uint32_t generateSeed(int levelNumber, uint32_t baseSeed = 12345) {
        return baseSeed + levelNumber * 7919;
    }
    
private:
    void generateWalls(GameController& game, int wallCount);
    void generateEnemies(GameController& game, int enemyCount, int levelNumber); // Добавлен параметр
    void generateTowers(GameController& game, int towerCount);
    Position findEmptyPosition(GameController& game);
};

#endif