#ifndef ENEMY_H
#define ENEMY_H

#include "Unit.h"

class Enemy : public Unit {
public:
    Enemy(int x = 0, int y = 0, int hp = 100, int damage = 20);
    ~Enemy() override = default;
};

#endif 