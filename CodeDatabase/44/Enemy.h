#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(int health, int damage, int x, int y);

    void Attack(Entity& target) override;

    void TakeSpellDamage(int damage, bool can_slow = false) override;
    std::string GetDescription() const override;
};
