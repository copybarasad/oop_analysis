#include "direct_damage.h"

Direct_damage::Direct_damage(unsigned int game_lvl): type(spell_type::direct_damage), 
        damage(3 + (game_lvl / 2)), range((5 + game_lvl) / 2) {};

Spell_stats Direct_damage::use_spell(unsigned int& upgrade){
    Spell_stats stats = Spell_stats(type);
    stats.damage = damage;
    stats.range = range + upgrade;
    upgrade = 0;
    return stats;
};

spell_type Direct_damage::get_type(){
    return type;
};