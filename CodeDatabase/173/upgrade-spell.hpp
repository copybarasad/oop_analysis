#ifndef UPGRADE_SPELL
#define UPGRADE_SPELL

#define UPGRADE_SPELL_BYTE 5

#include "spell.hpp"

class UpgradeSpell : public Spell {
  public:
    UpgradeSpell(int upgrades);
    bool canCast(Positionable *caster, Map *map, Field *target) override;
    void cast(Hand *hand, Positionable *caster, Map *map,
              Field *target) override;
    void upgrade() override;
    std::string toString() override;

    bytedump dump() override;
    UpgradeSpell(bytedump bytes);

  private:
    int m_Upgrades;
};

#endif /* UPGRADE_SPELL */
