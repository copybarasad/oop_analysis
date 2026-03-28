#pragma once

#include "Entity.hpp"

// Simple enemy; uses Entity stats directly.
class Enemy : public Entity {
public:
    Enemy() = default;
    Enemy(int hp, int atk, Pos p) : Entity(hp, atk, p) {}
};
