#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(std::string name = "Enemy", int hp = 15, int dmg = 4)
        : Entity(std::move(name), hp, dmg) {}

    void Update(GameField& field) override;
};
