#ifndef DIRECT_DAMAGE_SPELL_HPP
#define DIRECT_DAMAGE_SPELL_HPP

#define DIRECT_SPELL_BYTE 2

#include "../field/field.hpp"
#include "../player.hpp"
#include "targetted-spell.hpp"

class DirectDamageSpell : public TargettedSpell {
  public:
    DirectDamageSpell(int damage, int radius);
    bool canCast(Positionable *caster, Map *map, Field *field) override;
    void cast(Hand *hand, Positionable *caster, Map *map,
              Field *target) override;
    void upgrade() override;
    std::string toString() override;

    bytedump dump() override;
    DirectDamageSpell(bytedump bytes);

  private:
    int m_Damage;
};

#endif /* DIRECT_DAMAGE_SPELL_HPP */
