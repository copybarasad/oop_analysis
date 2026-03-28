#ifndef SPELL_FACTORY_HPP
#define SPELL_FACTORY_HPP

#include "spell.hpp"
#include <memory>
#include <vector>

class SpellFactory {
public:
    static std::unique_ptr<Spell> createRandomSpell();
};

#endif