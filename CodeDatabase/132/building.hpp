#pragma once

#include "entity.hpp"
#include "has_hand.hpp"

class Building : public Entity, public HasHand {
protected:
    int cooldown;
    int timer; // TODO: пока изменяется в choose_move
public:
    Building(std::shared_ptr<ISpell> spell, int health_points, int cooldown, int timer=0);
    bool is_move_available() const override;
    void reset_timer();
    
    GameObject* clone() const = 0;
    virtual Move choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) = 0;
protected:
    void reset_temp_effects() override;
    void apply_effect(Effect &effect) override;
};