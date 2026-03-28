#include <cassert>

#include "hand.hpp"
#include "upgrade-spell.hpp"

UpgradeSpell::UpgradeSpell(int upgrades) { m_Upgrades = upgrades; }

bool UpgradeSpell::canCast(Positionable *caster, Map *map, Field *target) {
    return true;
}

void UpgradeSpell::cast(Hand *hand, Positionable *caster, Map *map,
                        Field *target) {
    hand->addUpgrades(m_Upgrades);
}

void UpgradeSpell::upgrade() { m_Upgrades++; }

bytedump UpgradeSpell::dump() {
    bytedump bytes;
    bytes.reserve(4);

    bytes.push_back(static_cast<std::byte>(UPGRADE_SPELL_BYTE));
    bytes.push_back(static_cast<std::byte>(m_Upgrades));
    bytes.push_back(static_cast<std::byte>(0));
    bytes.push_back(static_cast<std::byte>(0));

    return bytes;
}

UpgradeSpell::UpgradeSpell(bytedump bytes) {
    assert(bytes.size() == 4);
    assert(static_cast<int>(bytes[0]) == UPGRADE_SPELL_BYTE);

    m_Upgrades = static_cast<int>(bytes[1]);
}

std::string UpgradeSpell::toString() {
    return Template("UpgradeSpell[upgrades=%d]").populate(m_Upgrades);
}
