#include "call_ally.h"

Call_ally::Call_ally(unsigned int game_lvl): type(spell_type::call_ally), count_ally(1) {};

Spell_stats Call_ally::use_spell(unsigned int& upgrade){
    Spell_stats stats = Spell_stats(type);
    stats.count_ally = count_ally + upgrade;
    upgrade = 0;
    return stats;
};

spell_type Call_ally::get_type(){
    return type;
};