#ifndef DIRECT_DAMAGE_H
#define DIRECT_DAMAGE_H

#include "spell.h"

class Direct_damage: public Spell{
    protected:
    spell_type type;
    unsigned int damage;
    unsigned int range;

    public:
    Direct_damage(unsigned int game_lvl);

    Spell_stats use_spell(unsigned int& upgrade);

    spell_type get_type();
};

#endif