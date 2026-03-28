#include <cassert>
#include <ranges>
#include <stdexcept>

#include "../positionable.hpp"
#include "aoe-spell.hpp"
#include "hand.hpp"

AoESpell::AoESpell(int damage, int radius) : TargettedSpell(radius) {
    if (damage <= 0) {
        throw std::invalid_argument("damage must be positive");
    }

    m_Damage = damage;
}

void AoESpell::upgrade() {
    m_Damage += 2;
    TargettedSpell::upgrade();
}

void AoESpell::cast(Hand *hand, Positionable *caster, Map *map, Field *target) {
    Position targetPos = map->locateField(target);

    for (int x : {0, 1}) {
        for (int y : {0, 1}) {
            Position pos = Position{targetPos.x + x, targetPos.y + y};
            Field *field = map->at(pos);

            if (field != nullptr) {
                field->damage(m_Damage);
            }
        }
    }
}

bytedump AoESpell::dump() {
    bytedump bytes;
    bytes.reserve(4);

    bytes.push_back(static_cast<std::byte>(AOE_SPELL_BYTE));
    bytes.push_back(static_cast<std::byte>(m_Radius));
    bytes.push_back(static_cast<std::byte>(m_Damage));
    bytes.push_back(static_cast<std::byte>(0));

    return bytes;
}

AoESpell::AoESpell(bytedump bytes) : TargettedSpell(bytes) {
    assert(bytes.size() == 4);
    assert(static_cast<int>(bytes[0]) == AOE_SPELL_BYTE);

    m_Damage = static_cast<int>(bytes[2]);
}

std::string AoESpell::toString() {
    return Template("AoESpell[damage=%d, radius=%d]")
        .populate(m_Damage, m_Radius);
}
