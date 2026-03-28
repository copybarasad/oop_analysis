#include <random>

#include "factory.hpp"

SpellFactory::SpellFactory() { m_Level = 1; }

Spell *SpellFactory::create() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 5);

    int type = dis(gen);

    switch (type) {
    case AOE_SPELL_BYTE:
        return createAOE();
    case DIRECT_SPELL_BYTE:
        return createDirect();
    case SUMMON_SPELL_BYTE:
        return createSummon();
    case TRAP_SPELL_BYTE:
        return createTrap();
    case UPGRADE_SPELL_BYTE:
        return createUpgrade();
    default:
        std::abort();
    }
}

void SpellFactory::incrementLevel() { m_Level++; }

int SpellFactory::totalUpgrades() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    return m_Level + dis(gen);
}

AoESpell *SpellFactory::createAOE() {
    AoESpell *spell = new AoESpell(3, 5);
    int upgrades = totalUpgrades();

    while (upgrades--) {
        spell->upgrade();
    }

    return spell;
}

DirectDamageSpell *SpellFactory::createDirect() {
    DirectDamageSpell *spell = new DirectDamageSpell(5, 5);
    int upgrades = totalUpgrades();

    while (upgrades--) {
        spell->upgrade();
    }

    return spell;
}

SummonSpell *SpellFactory::createSummon() {
    SummonSpell *spell = new SummonSpell(2, 2, 5);
    int upgrades = totalUpgrades();

    while (upgrades--) {
        spell->upgrade();
    }

    return spell;
}

TrapSpell *SpellFactory::createTrap() {
    TrapSpell *spell = new TrapSpell(7, 5);
    int upgrades = totalUpgrades();

    while (upgrades--) {
        spell->upgrade();
    }

    return spell;
}

UpgradeSpell *SpellFactory::createUpgrade() {
    return new UpgradeSpell(totalUpgrades());
}
