#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(int start_x, int start_y, int health, int damage);
    void update(Game& game) override;
};

#endif