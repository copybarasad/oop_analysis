#pragma once

#include "entity.hpp"

class MovableEntity: public Entity {
protected:
    bool can_move;
    virtual void reset_temp_effects() override;
    virtual void apply_effect(Effect &effect) override;

public:
    MovableEntity(int health_points);
    MovableEntity(int health_points, bool can_move);

    bool is_move_available() const override;
    virtual Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) = 0;
    GameObject *clone() const = 0;
};