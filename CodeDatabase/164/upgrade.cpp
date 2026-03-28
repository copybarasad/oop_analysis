#include "upgrade.h"

Upgrade::Upgrade(unsigned int game_lvl): type(spell_type::call_ally) {};

Spell_stats Upgrade::use_spell(unsigned int& upgrade){
    Spell_stats stats = Spell_stats(type);
    upgrade++;
    return stats;
};

spell_type Upgrade::get_type(){
    return type;
};