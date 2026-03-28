#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "Position.h"

class EnemyTower
{
private:
    Position position;
    int health;
    int damage;
    int range;
    int cooldown;
    
public:
    EnemyTower(const Position &pos, int hp = 50, int dmg = 10, int rng = 3);
    const Position &getPosition() const;
    bool isAlive() const;
    void takeDamage(int d);
    int getHealth() const { return health; }
    bool canFire() const;
    void setCooldown(int cd);
    void tickCooldown();
    int getRange() const;
    int getDamage() const;
};

#endif