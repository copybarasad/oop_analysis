#pragma once

#include "building_image.hpp"

#include <random>
#include "building.hpp"

class EnemySpawner : public Building {
public:
    EnemySpawner();

    EnemySpawner(BuildingImage building_image);
    BuildingImage get_data_image();

    EnemySpawner(int cooldown);
    
    GameObject *clone() const override;

    Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) override;
};