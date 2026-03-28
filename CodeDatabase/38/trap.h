#pragma once
#include "entity.h"
#include "field.h"
class Trap : public Entity {
private:
    unsigned int damage;

public:
    Trap(int x, int y, unsigned int damage = 60) : damage(damage) {
        this->x = x;
        this->y = y;
        this->lives = 1; // 1 = active, 0 = consumed
    }

    unsigned int getDamage() const {
        return damage;
    }

    bool isActive() const {
        return lives > 0;
    }

    void trigger() {
        lives = 0; // deactivate trap
    }

    bool placeTrap(int x, int y, Field &field, std::vector<Trap> &traps);
};

