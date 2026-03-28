#ifndef ENEMY_H
#define ENEMY_H

#include "Direction.h"
#include <vector>

class Enemy {
private:
    int health;
    int damage;
    std::vector<Direction> patrolPattern;
    size_t currentPatrolStep;

public:
    Enemy(int health, int damage, const std::vector<Direction>& patrolPattern);
    
    int getHealth() const;
    int getDamage() const;
    const std::vector<Direction>& getPatrolPattern() const;
    size_t getCurrentPatrolStep() const;
    Direction getNextPatrolDirection();
    
    int takeDamage(int damage);
    bool isAlive() const;
};

#endif
