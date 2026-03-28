#ifndef GAME_INITIALIZER_H
#define GAME_INITIALIZER_H

#include "game_field.h"
#include "enemy.h"
#include "enemy_tower.h"
#include <vector>

class GameInitializer {
public:
    static void initializeEnemies(std::vector<Enemy>& enemies, const GameField& field, int enemyCount);
    static void initializeTowers(std::vector<EnemyTower>& towers, const GameField& field, int count = 1);
    
    static void validateEnemies(std::vector<Enemy>& enemies, const GameField& field);
    static void validateTowers(std::vector<EnemyTower>& towers, const GameField& field);
};

#endif