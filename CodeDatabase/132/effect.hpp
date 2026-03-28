#pragma once

#include "effect_image.hpp"

class Effect {
protected:
    bool initial_damage_done = false;

    int lifetime;
    int initial_damage;
    int tick_damage; 
    bool slowing_down = false;
    
public:
    std::string name;

    Effect(int initial_damage, std::string name);
    Effect(int initial_damage, int tick_damage, bool slowing_down, std::string name);
    Effect(int initial_damage, int tick_damage, bool slowing_down, std::string name, int lifetime);
    Effect(const EffectImage& img);
    EffectImage get_data_image() const;

    bool operator<(const Effect& other) const;

    int tick();
    bool expired();
    bool is_slowing_down();

    int apply_to_health(int cur_health_points);
};