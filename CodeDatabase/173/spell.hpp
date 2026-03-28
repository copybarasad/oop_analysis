#ifndef SPELL_HPP
#define SPELL_HPP

#include "../field/field.hpp"
#include "../map/map.hpp"
#include "../utils/template.hpp"

class Hand;

class Spell : public Dumpable {
  public:
    static Spell *fromByteDump(bytedump bytes);

    virtual bool canCast(Positionable *caster, Map *map, Field *target) = 0;
    virtual void cast(Hand *hand, Positionable *caster, Map *map,
                      Field *target) = 0;
    virtual void upgrade() = 0;
    virtual std::string toString() = 0;
};

#endif /* SPELL_HPP */
