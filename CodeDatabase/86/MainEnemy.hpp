#pragma once
#include "Enemy.hpp"
#include "EnemyConstruction.hpp"
#include "Grid.hpp"

class MainEnemy : public Character{
public:
    MainEnemy(int hp, int damage, Position pos, Logger& logger);
    bool chase(Grid& grid, Player& player);
    std::unique_ptr<EnemyConstruction> putConstruction(Position pos, Grid& grid, Logger& logger);
};
