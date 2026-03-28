#ifndef SPELL_FACTORY_HPP
#define SPELL_FACTORY_HPP

#include "aoe-spell.hpp"
#include "direct-damage-spell.hpp"
#include "spell.hpp"
#include "summon-spell.hpp"
#include "trap-spell.hpp"
#include "upgrade-spell.hpp"

class SpellFactory {
  public:
    SpellFactory();

    Spell *create();
    void incrementLevel();

  private:
    int m_Level;

    int totalUpgrades();

    AoESpell *createAOE();
    DirectDamageSpell *createDirect();
    SummonSpell *createSummon();
    TrapSpell *createTrap();
    UpgradeSpell *createUpgrade();
};

#endif /* SPELL_FACTORY_HPP */
