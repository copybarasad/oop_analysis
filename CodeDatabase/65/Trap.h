#pragma once

#include "GameObject.h"
#include "Enemy.h"
#include <memory>

class Trap : public GameObject {
    int damage;
    bool activated = false;
    
public:
    Trap(int x, int y, int damage, char sprite = '^', std::string color = "\033[33m")
        : GameObject(x, y, sprite, color, false, ObjectTag::TAG_TRAP),
          damage(damage) {}

    int getDamage() const { return damage; }
    bool isActivated() const { return activated; }

    void activate(Enemy* enemy) {
        if (!activated && enemy) {
            enemy->takeDamage(damage);
            activated = true;
            sprite = 'x';
            color = "\033[31m";
        }
    }

    std::unique_ptr<GameObject> clone() const override {
        auto trap = std::make_unique<Trap>(x, y, damage, sprite, color);
        const_cast<Trap*>(trap.get())->activated = activated;
        return trap;
    }
};
