#include <bit>
#include <cassert>
#include <cstdlib>
#include <stdexcept>

#include "targetted-spell.hpp"

TargettedSpell::TargettedSpell(int radius) {
    if (radius <= 0) {
        throw std::invalid_argument("radius must be positive");
    }

    m_Radius = radius;
}

bool TargettedSpell::canCast(Positionable *caster, Map *map, Field *target) {
    Position p1 = caster->getPosition();
    Position p2 = map->locateField(target);

    return map->distance(p1, p2) <= (double)m_Radius;
}

void TargettedSpell::upgrade() { m_Radius++; }

bytedump TargettedSpell::dump() {
    bytedump bytes;
    bytes.reserve(4);

    bytes.push_back(static_cast<std::byte>(TARGETTED_SPELL_BYTE));
    bytes.push_back(static_cast<std::byte>(m_Radius));
    bytes.push_back(static_cast<std::byte>(0));
    bytes.push_back(static_cast<std::byte>(0));

    return bytes;
}

TargettedSpell::TargettedSpell(bytedump bytes) {
    assert(bytes.size() == 4);
    m_Radius = static_cast<int>(bytes[1]);
}

std::string TargettedSpell::toString() { return ""; }
