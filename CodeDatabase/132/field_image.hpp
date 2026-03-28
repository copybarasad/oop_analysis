#pragma once

#include <map>
#include <vector>
#include "position.hpp"

#include "player_image.hpp"
#include "mob_image.hpp"
#include "building_image.hpp"
#include "trap_image.hpp"

class FieldImage {
public:
    int height;
    int width;

    // field
    std::map<Position, PlayerImage> players;
    std::map<Position, MobImage> enemies;
    std::map<Position, MobImage> allies;
    std::map<Position, BuildingImage> enemy_towers;
    std::map<Position, BuildingImage> enemy_spawners;
    std::map<Position, TrapImage> traps;

    //landscape
    std::vector<Position> stones;
    std::vector<Position> swamps; 
};