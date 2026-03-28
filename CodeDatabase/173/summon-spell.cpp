#include <cassert>

#include "../ally.hpp"
#include "hand.hpp"
#include "summon-spell.hpp"

SummonSpell::SummonSpell(int hp, int damage, int radius)
    : TargettedSpell(radius) {
    m_Damage = damage;
    m_SummonHP = hp;
}

void SummonSpell::upgrade() {
    TargettedSpell::upgrade();
    m_Damage++;
    m_SummonHP++;
}

bool SummonSpell::canCast(Positionable *caster, Map *map, Field *target) {
    return TargettedSpell::canCast(caster, map, target) && target->canOccupy();
}

void SummonSpell::cast(Hand *hand, Positionable *caster, Map *map,
                       Field *target) {
    Creature *summon = new Ally(m_SummonHP, m_SummonHP);
    Position spawnPoint = map->locateField(target);

    summon->setWeapon(Weapon{WeaponType::Melee, m_Damage});
    map->spawn(summon, spawnPoint);
}

bytedump SummonSpell::dump() {
    bytedump bytes;
    bytes.reserve(4);

    bytes.push_back(static_cast<std::byte>(SUMMON_SPELL_BYTE));
    bytes.push_back(static_cast<std::byte>(m_Radius));
    bytes.push_back(static_cast<std::byte>(m_Damage));
    bytes.push_back(static_cast<std::byte>(m_SummonHP));

    return bytes;
}

SummonSpell::SummonSpell(bytedump bytes) : TargettedSpell(bytes) {
    assert(bytes.size() == 4);
    assert(static_cast<int>(bytes[0]) == SUMMON_SPELL_BYTE);

    m_Damage = static_cast<int>(bytes[2]);
    m_SummonHP = static_cast<int>(bytes[3]);
}

std::string SummonSpell::toString() {
    return Template("SummonSpell[HP=%d, damage=%d, radius=%d]")
        .populate(m_SummonHP, m_Damage, m_Radius);
}
