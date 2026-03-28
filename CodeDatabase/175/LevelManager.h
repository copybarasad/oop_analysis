#pragma once
#include <memory>
#include "GameState.h"

struct LevelConfig {
    int width;
    int height;
    int enemyCount;
    int baseCount;
    int towerCount;
    double wallProbability;
    double slowProbability;
    int enemyHp;
    int enemyDamage;
};

class LevelManager {
public:
    static std::unique_ptr<GameState> createLevel(int level, std::shared_ptr<Player> existingPlayer = nullptr);
    static LevelConfig getLevelConfig(int level);
    
private:
    static void placeEnemies(std::shared_ptr<Field> field, int count, int hp, int damage);
    static void placeBases(std::shared_ptr<Field> field, int count);
    static void placeTowers(std::shared_ptr<Field> field, int count);
    static std::pair<int, int> findEmptyCell(std::shared_ptr<Field> field);
};