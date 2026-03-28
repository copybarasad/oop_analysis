#ifndef SPELL_H
#define SPELL_H

#include "spell_stats.h"

class Spell{ 
    public:
    virtual Spell_stats use_spell(unsigned int& upgrade) = 0;
    virtual spell_type get_type() = 0;
};

#endif