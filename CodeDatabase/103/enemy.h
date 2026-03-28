#pragma once
#include "entity.h"
#include <stdexcept>

class Enemy : public Entity {
public:
    Enemy(int startHealth = 30, int startDamage = 5);
    void takeDamage(int damage) override;
};