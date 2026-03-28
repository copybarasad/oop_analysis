#ifndef TOWER_H
#define TOWER_H

#include "Unit.h"

class Tower : public Unit {
public:
    Tower(int x = 0, int y = 0, int damage = 15, int range = 3, int hp = 100, int cooldown = 1);

    int getRange() const;
    int getLastAttackTurn() const;
    void setLastAttackTurn(int t);

    int getAttackCooldown() const;

private:
    int range_;
    int lastAttackTurn_;
    int attackCooldown_;
};

#endif // TOWER_H
