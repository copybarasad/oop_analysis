#pragma once

#include "mob_image.hpp"

#include <random>
#include "movable_entity.hpp"
#include "armed.hpp"
#include "arms.hpp"
 
class Ally : public MovableEntity, public Armed {
public:
    Ally();

    Ally(MobImage ally_image);
    MobImage get_data_image();

    GameObject* clone() const override;
    Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) override;
};