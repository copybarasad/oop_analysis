#ifndef DAMAGE_SPELL_H
#define DAMAGE_SPELL_H

#include <cmath>
#include "../../RangedSpell/RangedSpell.hpp"

class DamageSpell : public RangedSpell {
    protected:
    unsigned int damage;

    DamageSpell (Field& field, size_t range, unsigned int damage, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target);

    public:
    unsigned int getDamage () const;

    protected:
    void dealDamage (CastResult& result, size_t x, size_t y);
};

#endif