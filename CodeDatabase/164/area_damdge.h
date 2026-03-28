#ifndef AREA_DAMAGE_H 
#define AREA_DAMAGE_H

#include "spell.h"

class Area_damage: public Spell{
    protected:
    spell_type type;
    unsigned int damage;
    unsigned int range;
    unsigned int area_side;

    public:
    Area_damage(unsigned int game_lvl);

    Spell_stats use_spell(unsigned int& upgrade);

    spell_type get_type();
};

#endif