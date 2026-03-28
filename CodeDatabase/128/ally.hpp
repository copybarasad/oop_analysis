#pragma once
#include "entity.hpp"

class Ally : public Entity {
public:
    Ally(int x, int y, int health, int damage)
    : Entity(health, damage), x_(x), y_(y) {}

    int getX() const noexcept { return x_; }
    int getY() const noexcept { return y_; }
    int getHealth() const { return health_; }
    int getDamage() const { return damage_; }

    void setPosition(int x, int y) noexcept { x_ = x; y_ = y; }
private:
    int x_;
    int y_;
};
