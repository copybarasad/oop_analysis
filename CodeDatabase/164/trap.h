#ifndef TRAP_H 
#define TRAP_H

#include "spell.h"

class Trap: public Spell{
    protected:
    spell_type type;
    unsigned int damage;

    public:
    Trap(unsigned int game_lvl);

    Spell_stats use_spell(unsigned int& upgrade);

    spell_type get_type();
};

#endif