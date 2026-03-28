#ifndef SPELL_FACTRORY_H
#define SPELL_FACTRORY_H

#include "spell.hpp"

class SpellFactory{

    public:
        static Spell* createRandomSpell();
        static Spell* createSpell(int typeId);
};

#endif