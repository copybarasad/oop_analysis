#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include "Spell.h"
#include <random>
#include <istream>

class SpellFactory {
private:
    std::mt19937 randomGenerator;

public:
    SpellFactory();

    Spell *createRandomSpell();

    Spell *createSpell(SpellType type);

    Spell *createSpell(SpellType type, std::istream &in);
};

#endif
