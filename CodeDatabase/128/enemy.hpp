#pragma once
#include "entity.hpp"

class Player;

class Enemy : public Entity {
public:
    Enemy(int health, int damage);
    void Attack(Entity& target) override;
};