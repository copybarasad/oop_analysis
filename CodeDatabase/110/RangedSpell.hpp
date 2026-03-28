#ifndef RANGED_SPELL_H
#define RANGED_SPELL_H

#include "../../BaseSpell/Spell.hpp"

class RangedSpell : public Spell {
    protected:
    Field& field;
    size_t range;
    size_t casterX, casterY;
    size_t targetX, targetY;

    RangedSpell (Field& field, size_t range, std::pair<size_t, size_t> caster, std::pair<size_t, size_t> target);

    size_t getRange () const;
    void setCaster (std::pair<size_t, size_t> caster);
    void setTarget (std::pair<size_t, size_t> target);

    protected:
    virtual bool canCast (size_t targetX, size_t targetY) const = 0;
    size_t calculateDistance (size_t x, size_t y) const;
};

#endif