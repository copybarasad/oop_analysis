#ifndef TRAP_SPELL_HPP
#define TRAP_SPELL_HPP

#include "spell.hpp"
#include "map.hpp"

class TrapSpell : public Spell {
public:
    TrapSpell();
    bool cast(Map* map) override;
};

#endif