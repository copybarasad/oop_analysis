#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "Position.h"
#include "Enemy.h"

class EnemyBuilding
{
private:
    Position position;
    int spawnCounter;
    int spawnInterval;

public:
    EnemyBuilding(const Position &buildPosition, int interval);
    Position getPosition() const;
    bool shouldSpawnEnemy() const;
    void update();
    Enemy spawnEnemy();
};

#endif