#pragma once
#include "entity.hpp"
#include <cstddef>

class Tower : public Entity {
public:
    int current_timer;
    int cooldown;
    size_t level;
    Tower(int number, int temp_level, int temp_id);

    Tower(Tower&& other) noexcept;
    Tower& operator=(Tower&& other) noexcept;
    Tower(const Tower&) = delete;
    Tower& operator=(const Tower&) = delete;
};