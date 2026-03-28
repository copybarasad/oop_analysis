#ifndef AOE_SPELL_HPP
#define AOE_SPELL_HPP

#define AOE_SPELL_BYTE 1

#include "../field/field.hpp"
#include "../map/map.hpp"
#include "../player.hpp"
#include "targetted-spell.hpp"

class AoESpell : public TargettedSpell {
  public:
    AoESpell(int damage, int radius);
    void cast(Hand *hand, Positionable *caster, Map *map,
              Field *target) override;
    void upgrade() override;
    std::string toString() override;

    bytedump dump() override;
    AoESpell(bytedump bytes);

  private:
    int m_Damage;
};

#endif /* AOE_SPELL_HPP */
