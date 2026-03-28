#pragma once

#include <algorithm>

#include "player_image.hpp"

#include "rifle.hpp"
#include "sword.hpp"
#include "controls.hpp"
#include "movable_entity.hpp"
#include "armed.hpp"
#include "has_hand.hpp"


class Player : public MovableEntity, public Armed, public HasHand {
public:
    Player();
    
    Player(PlayerImage player_image);
    PlayerImage get_data_image();

    GameObject* clone() const override;
    Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) override;
};