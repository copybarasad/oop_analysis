#ifndef DIRECT_SPELL_HPP
#define DIRECT_SPELL_HPP

#include "spell.hpp"
#include "map.hpp"

class DirectSpell : public Spell {
public:
    DirectSpell();
    bool cast(Map* map) override;
    void shoot(Map* map, int x, int y, int damage);
};

#endif