#pragma once
#include "entity.hpp"
#include <iostream>

class Game;

class EnemyStructure : public Entity {
public:
    EnemyStructure(int x, int y, int health, int damage, int delay);
    virtual ~EnemyStructure() = default;

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getDelay() const noexcept { return delay_; }
    int getCounter() const noexcept { return counter_; }
    void setCounter(int c) noexcept { counter_ = c; }

    void Attack(Entity&) override {}
    virtual void Tick(Game& game) = 0;
    
protected:
    int x_, y_;
    int delay_;
    int counter_;
};
