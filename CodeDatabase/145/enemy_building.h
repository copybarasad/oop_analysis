#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "game_entity.h"
#include <utility>

class Enemy;

class EnemyBuilding : public GameEntity {
private:
    std::pair<int, int> position;
    int spawnCounter;
    int spawnInterval;
    
public:
    EnemyBuilding(int x, int y, int spawnInterval);
    std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    bool shouldSpawnEnemy() const;
    void update();
    Enemy spawnEnemy() const;
    int getScore() const;
    int getSpawnInterval() const;
    int getSpawnCounter() const;
    void setSpawnCounter(int c);
};

#endif