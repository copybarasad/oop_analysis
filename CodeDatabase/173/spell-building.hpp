#ifndef SPELL_BUILDING_HPP
#define SPELL_BUILDING_HPP

#define SPELL_BUILDING_BYTE 2

#include "generic-building.hpp"

class Creature;
class TargettedSpell;

class SpellBuilding : public GenericBuilding {
  public:
    SpellBuilding(int hp);
    SpellBuilding(const SpellBuilding &other);
    Building *clone() override;

    void setTarget(Creature *target);
    void setSpell(TargettedSpell *spell);
    void setAttackRate(int attackRate);
    void update() override;
    void setMap(Map *map) override;

    std::string chr() override;
    Color color() override;

    bytedump dump() override;
    SpellBuilding(bytedump bytes);

  private:
    Creature *m_Target;
    TargettedSpell *m_Spell;
    Map *m_Map;
    int m_AttackRate;
    int m_TickCount;
};

#endif /* SPELL_BUILDING_HPP */
