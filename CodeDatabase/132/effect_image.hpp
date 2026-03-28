#pragma once

#include "string"

class EffectImage {
public:
    std::string name;

    bool initial_damage_done;
    int lifetime;
    int initial_damage;
    int tick_damage; 
    bool slowing_down;

    EffectImage(
        std::string name,
        bool initial_damage_done, 
        int lifetime, 
        int initial_damage, 
        int tick_damage, 
        bool slowing_down
    ):
        name(name),
        initial_damage_done(initial_damage_done),
        lifetime(lifetime),
        initial_damage(initial_damage),
        tick_damage(tick_damage),
        slowing_down(slowing_down)
    {}
};