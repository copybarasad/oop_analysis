#ifndef ALLY_H
#define ALLY_H

#include "Character.h"
#include "Position.h"
#include <vector>
#include "Enemy.h"

class Ally : public Character
{
public:
    Ally(const Position &pos, int health, int dmg);
    void move(const Position &newPosition);
    void act(std::vector<Enemy> &enemies);
};

#endif