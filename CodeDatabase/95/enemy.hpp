#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity.hpp"

class Enemy : public Entity {
public:
    Enemy(int x, int y, int hp_ = 5, int dmg = 3);

};

#endif