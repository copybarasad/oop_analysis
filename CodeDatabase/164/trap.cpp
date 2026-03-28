#include "trap.h"

Trap::Trap(unsigned int game_lvl): type(spell_type::trap), damage(2 + game_lvl), range(1) {};

Spell_stats Trap::use_spell(unsigned int& upgrade){
    Spell_stats stats = Spell_stats(type);
    stats.damage = damage + upgrade;
    upgrade = 0;
    return stats;
};

spell_type Trap::get_type(){
    return type;
};