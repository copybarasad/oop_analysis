#ifndef TARGETTED_SPELL_HPP
#define TARGETTED_SPELL_HPP

#define TARGETTED_SPELL_BYTE 0

#include "spell.hpp"

class TargettedSpell : public Spell {
  public:
    TargettedSpell(int radius);
    bool canCast(Positionable *caster, Map *map, Field *target) override;
    void upgrade() override;
    std::string toString() override;

    bytedump dump() override;
    TargettedSpell(bytedump bytes);

  protected:
    int m_Radius;
};

#endif /* TARGETTED_SPELL_HPP */
