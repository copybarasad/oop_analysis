#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "position.h"
#include <vector>

class GameMap;
class Enemy;

class EnemyBuilding {
private:
    Position position;
    int level;
    int count;
    int interval;
    int spawnedEnemyHealth;  
    int spawnedEnemyDamage;  
    const int TOTAL_SPAWN_DIRECTIONS = 8;

public:
    EnemyBuilding(int x, int y, 
                  int level = 1,
                  int interval = 5, 
                  int enemyHealth = 50, 
                  int enemyDamage = 5);
    
    const Position& getPosition() const;
    void spawn(std::vector<Enemy*>& enemies, GameMap& map);
    ~EnemyBuilding() = default;
    int getSpawnInterval(){
        return interval;
    }

    int getSpawnCount(){
        return count;
    }
};

#endif