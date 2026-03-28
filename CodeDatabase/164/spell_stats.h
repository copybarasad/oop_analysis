#ifndef SPELL_STATS_H 
#define SPELL_STATS_H

enum class spell_type{
    direct_damage,
    area_damage,
    trap,
    call_ally,
    upgrade
};

class Spell_stats{
    public:
    spell_type type;
    unsigned int damage;
    unsigned int range;
    unsigned int area_side;
    unsigned int count_ally;

    Spell_stats(spell_type type);
};

#endif