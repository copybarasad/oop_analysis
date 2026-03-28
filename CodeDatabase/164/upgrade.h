#ifndef UPGRADE_H 
#define UPGRADE_H

#include "spell.h"

class Upgrade: public Spell{
    protected:
    spell_type type;

    public:
    Upgrade(unsigned int game_lvl);

    Spell_stats use_spell(unsigned int& upgrade);

    spell_type get_type();
};

#endif