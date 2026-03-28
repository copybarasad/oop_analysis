#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character
{
public:
    Enemy(const Position &startPosition, int health, int damage);
    void move(const Position &newPosition) override;
};

#endif