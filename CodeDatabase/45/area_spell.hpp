#ifndef AREA_SPELL_HPP
#define AREA_SPELL_HPP

#include "spell.hpp"
#include "map.hpp"

class AreaSpell : public Spell {
public:
    AreaSpell();
    bool cast(Map* map) override;
};

#endif