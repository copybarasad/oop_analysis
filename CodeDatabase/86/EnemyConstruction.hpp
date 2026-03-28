#pragma once
#include "Enemy.hpp"
#include "Position.hpp"
#include "Grid.hpp"
#include <memory>
#include <iostream> 


class EnemyConstruction {
    int hp_;
    int spawnRate_;
    Position pos_;
    Grid& grid_;
    Logger& logger_;

public:
    EnemyConstruction(int rate, Position pos, Grid& grid, Logger& logger);
    std::unique_ptr<Enemy> spawnNewEnemy();
    bool isAlive() const;
    void takeDamage(int damage);
    int getRate() const;
    Position getPos() const;

    void save(std::ostream& os) const;
    void load(std::istream& is);
};
