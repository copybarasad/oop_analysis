#pragma once

#include "building_image.hpp"

#include <random>
#include "building.hpp"

class EnemyTower : public Building {
public:
    EnemyTower();

    EnemyTower(BuildingImage building_image);
    BuildingImage get_data_image();

    GameObject *clone() const override;
    Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) override;
};
