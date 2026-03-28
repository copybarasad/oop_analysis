#ifndef SUMMON_SPELL_HPP
#define SUMMON_SPELL_HPP

#define SPELL_SUMMON_MAX_ATTEMPTS 10
#define SUMMON_SPELL_BYTE 3

#include "targetted-spell.hpp"

class SummonSpell : public TargettedSpell {
  public:
    SummonSpell(int hp, int damage, int radius);
    bool canCast(Positionable *caster, Map *map, Field *target) override;
    void cast(Hand *hand, Positionable *caster, Map *map,
              Field *target) override;
    void upgrade() override;
    std::string toString() override;

    bytedump dump() override;
    SummonSpell(bytedump bytes);

  private:
    int m_Damage;
    int m_SummonHP;
};

#endif /* SUMMON_SPELL_HPP */
