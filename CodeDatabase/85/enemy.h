#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character {
public:
    Enemy(int health, int damage_value, Position start_position);
};

#endif