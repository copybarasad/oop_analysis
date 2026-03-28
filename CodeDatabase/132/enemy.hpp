#pragma once

#include "mob_image.hpp"

#include <random>
#include "movable_entity.hpp"
#include "armed.hpp"
#include "arms.hpp"
 
class Enemy : public MovableEntity, public Armed {
public:
    Enemy();

    Enemy(MobImage enemy_image);
    MobImage get_data_image();

    Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) override;
    GameObject* clone() const override;
};