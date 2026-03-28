#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Enemy : public Entity {
public:
    Enemy(int hp, int dmg);
};

#endif