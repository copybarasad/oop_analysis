#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "position.h"

class EnemyBuilding {
private:
    Position position;
    int spawnCooldown;
    int currentCooldown;
    
    void validateCooldown();

public:
    EnemyBuilding(int cooldown = 3);
    
    Position getPosition() const;
    void setPosition(const Position& newPosition);
    
    bool canSpawnEnemy() const;
    void update();
    void resetCooldown();
};

#endif