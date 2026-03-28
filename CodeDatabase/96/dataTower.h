#pragma once
#include <memory>

struct dataTower{
    std::pair<int,int> pos;
    int damageCooldown;
    int damageInterval;
    int range;
    int damage;
    int health;
    int stunDuration;
};