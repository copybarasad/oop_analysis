#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

class Enemy : public Character {
public:
    Enemy(int health = 5, int damage = 1);
};

#endif