#ifndef  CALL_ALLY_H 
#define CALL_ALLY_H

#include "spell.h"

class Call_ally: public Spell{
    protected:
    spell_type type;
    unsigned int count_ally;

    public:
    Call_ally(unsigned int game_lvl);

    Spell_stats use_spell(unsigned int& upgrade);

    spell_type get_type();
};

#endif