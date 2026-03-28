#pragma once
#include "Character.hpp"
#include "Player.hpp"
#include "Grid.hpp"

class Enemy : public Character {
public:
    Enemy(int hp, int damage, Position pos, Logger& logger);
    bool chase(Grid& grid, Player& player);
};
