#include <cassert>
#include <stdexcept>

#include "direct-damage-spell.hpp"
#include "hand.hpp"

DirectDamageSpell::DirectDamageSpell(int damage, int radius)
    : TargettedSpell(radius) {
    if (damage < 0) {
        throw std::invalid_argument("damage must be positive");
    }

    m_Damage = damage;
}

bool DirectDamageSpell::canCast(Positionable *caster, Map *map, Field *target) {
    return TargettedSpell::canCast(caster, map, target) && target->canAttack();
}

void DirectDamageSpell::cast(Hand *hand, Positionable *caster, Map *map,
                             Field *target) {
    target->damage(m_Damage);
}

void DirectDamageSpell::upgrade() {
    m_Damage += 2;
    TargettedSpell::upgrade();
}

bytedump DirectDamageSpell::dump() {
    bytedump bytes;
    bytes.reserve(4);

    bytes.push_back(static_cast<std::byte>(DIRECT_SPELL_BYTE));
    bytes.push_back(static_cast<std::byte>(m_Radius));
    bytes.push_back(static_cast<std::byte>(m_Damage));
    bytes.push_back(static_cast<std::byte>(0));

    return bytes;
}

DirectDamageSpell::DirectDamageSpell(bytedump bytes) : TargettedSpell(bytes) {
    assert(bytes.size() == 4);
    assert(static_cast<int>(bytes[0]) == DIRECT_SPELL_BYTE);

    m_Damage = static_cast<int>(bytes[2]);
}

std::string DirectDamageSpell::toString() {
    return Template("DirectDamageSpell[damage=%d, radius=%d]")
        .populate(m_Damage, m_Radius);
}
