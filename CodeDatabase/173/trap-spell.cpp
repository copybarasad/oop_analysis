#include <cassert>
#include <stdexcept>

#include "../trap/damage-trap.hpp"
#include "trap-spell.hpp"

TrapSpell::TrapSpell(int damage, int radius) : TargettedSpell(radius) {
    if (damage <= 0) {
        throw std::invalid_argument("damage must be positive");
    }

    m_Damage = damage;
}

bool TrapSpell::canCast(Positionable *caster, Map *map, Field *target) {
    return TargettedSpell::canCast(caster, map, target) &&
           target->canPlaceTrap();
}

void TrapSpell::cast(Hand *hand, Positionable *caster, Map *map,
                     Field *target) {
    DamageTrap *trap = new DamageTrap(m_Damage);
    target->placeTrap(trap);
}

void TrapSpell::upgrade() {
    m_Damage += 2;
    TargettedSpell::upgrade();
}

TrapSpell::TrapSpell(bytedump bytes) : TargettedSpell(bytes) {
    assert(bytes.size() == 4);
    assert(static_cast<int>(bytes[0]) == TRAP_SPELL_BYTE);

    m_Damage = static_cast<int>(bytes[2]);
}

bytedump TrapSpell::dump() {
    bytedump bytes;
    bytes.reserve(4);

    bytes.push_back(static_cast<std::byte>(TRAP_SPELL_BYTE));
    bytes.push_back(static_cast<std::byte>(m_Radius));
    bytes.push_back(static_cast<std::byte>(m_Damage));
    bytes.push_back(static_cast<std::byte>(0));

    return bytes;
}

std::string TrapSpell::toString() {
    return Template("TrapSpell[damage=%d, radius=%d]")
        .populate(m_Damage, m_Radius);
}
