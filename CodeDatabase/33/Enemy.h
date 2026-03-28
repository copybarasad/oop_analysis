#pragma once
#include "Character.h"

class Enemy : public Character {
public:
    Enemy(int hp, int damage, Coord pos);
};
