#pragma once
#include "entity.hpp"

class Enemy : public Entity {
public:
    bool skip_turn;
    Enemy(int temp_level, int number);

    Enemy(Enemy&& other) noexcept;
    Enemy& operator=(Enemy&& other) noexcept;
    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
};