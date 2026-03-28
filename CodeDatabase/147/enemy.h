#ifndef ENEMY_H
#define ENEMY_H

#include "unit.h"

class Enemy : public Unit {
public:
    explicit Enemy(int health = 50, int damage = 10);
private:
};

#endif // ENEMY_H