#pragma once

#include <vector>

#include "game_object.hpp"
#include "effect.hpp"
#include "IEntity.hpp"

class Entity : public virtual GameObject, public IEntity {
protected:
    int health_points;
    std::vector<Effect> applied_effects;
public:
    Entity(int health_points);

    void add_effects(std::set<Effect> effects, bool immediate_apply=false) override;
    void add_effect(Effect effect, bool immediate_apply=false) override;
    void apply_effects() override;
    bool is_alive() const override;

    virtual bool is_move_available() const = 0;
    virtual Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) = 0;
    GameObject *clone() const = 0;
protected:
    std::vector<EffectImage> get_effects_img();
    virtual void reset_temp_effects() = 0;
    virtual void apply_effect(Effect &effect) = 0;
};