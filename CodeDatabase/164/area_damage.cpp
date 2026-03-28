#include "area_damage.h"

Area_damage::Area_damage(unsigned int game_lvl): type(spell_type::area_damage), 
        damage(2 + (game_lvl / 2)), range((5 + game_lvl) / 2), area_side(2) {};

Spell_stats Area_damage::use_spell(unsigned int& upgrade){
    Spell_stats stats = Spell_stats(type);
    stats.damage = damage;
    stats.range = range;
    stats.area_side = area_side + upgrade;
    upgrade = 0;
    return stats;
};

spell_type Area_damage::get_type(){
    return type;
};