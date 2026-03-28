#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "character.hpp"

class Enemy: public Character{
    public:
        Enemy(int health, int damage_value, Position start_position);
};

#endif