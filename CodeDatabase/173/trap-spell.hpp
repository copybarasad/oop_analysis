#ifndef TRAP_SPELL_HPP
#define TRAP_SPELL_HPP

#define TRAP_SPELL_BYTE 4

#include "targetted-spell.hpp"

class TrapSpell : public TargettedSpell {
  public:
    TrapSpell(int damage, int radius);
    bool canCast(Positionable *caster, Map *map, Field *target) override;
    void cast(Hand *hand, Positionable *caster, Map *map,
              Field *target) override;
    void upgrade() override;
    std::string toString() override;

    bytedump dump() override;
    TrapSpell(bytedump bytes);

  private:
    int m_Damage;
};

#endif /* TRAP_SPELL_HPP */
