#pragma once
#include <memory>


struct dataBuilding{
    std::pair<int,int> pos;
    int spawnCooldown;
    int spawnInterval;
    int damage;
    int health;
    int stunDuration;
};