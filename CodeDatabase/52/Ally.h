#pragma once
#include "Entity.h"

class Ally : public Entity {
public:
    Ally(std::string name = "Ally", int hp = 10, int dmg = 3)
        : Entity(std::move(name), hp, dmg) {}

    void Update(class GameField& field) override;
};