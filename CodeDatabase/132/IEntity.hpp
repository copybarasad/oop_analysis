#pragma once

#include <vector>
#include <map>

#include "IGameObject.hpp"
#include "move.hpp"

class IEntity {
public:
    virtual void add_effects(std::set<Effect> effects, bool immediate_apply=false) = 0;
    virtual void add_effect(Effect effect, bool immediate_apply=false) = 0;
    virtual void apply_effects() = 0;
    virtual bool is_move_available() const = 0;
    virtual Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) = 0;
    virtual bool is_alive() const = 0;
};