#pragma once

#include <set>
#include "effect.hpp"

enum CombatMode {
    NONE,
    MELEE,
    LONG_RANGE    
};

class Weapon
{
private:
    CombatMode combat_type;
    std::set<Effect> bullet_effects;
public:
    Weapon(CombatMode combat_type, std::set<Effect> bullet_effects);
    std::set<Effect> use();
    CombatMode combat_mode();
};