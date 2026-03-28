#include "spell-building.hpp"

#include "../creature.hpp"
#include "../spell/targetted-spell.hpp"

SpellBuilding::SpellBuilding(int hp) : GenericBuilding(hp) {
    m_AttackRate = 3;
    m_TickCount = 0;
}

SpellBuilding::SpellBuilding(const SpellBuilding &building)
    : GenericBuilding(building) {}

Building *SpellBuilding::clone() { return new SpellBuilding(*this); }

void SpellBuilding::setSpell(TargettedSpell *spell) { m_Spell = spell; }

void SpellBuilding::setTarget(Creature *target) { m_Target = target; }

void SpellBuilding::setMap(Map *map) { m_Map = map; }

void SpellBuilding::setAttackRate(int attackRate) { m_AttackRate = attackRate; }

void SpellBuilding::update() {
    m_TickCount = (m_TickCount + 1) % m_AttackRate;

    if (m_TickCount != 0) {
        return;
    }

    Position targetPos = m_Target->getPosition();
    Field *target = m_Map->at(targetPos);

    if (m_Spell->canCast(this, m_Map, target)) {
        m_Spell->cast(nullptr, this, m_Map, target);
    }
}

std::string SpellBuilding::chr() { return "¥"; }
Color SpellBuilding::color() { return Color::red(); }

bytedump SpellBuilding::dump() {
    bytedump bytes;
    bytes.reserve(16);

    bytes.push_back(static_cast<std::byte>(SPELL_BUILDING_BYTE));
    bytes.push_back(static_cast<std::byte>(m_HP));
    bytes.push_back(static_cast<std::byte>(m_TickCount));
    bytes.push_back(static_cast<std::byte>(m_AttackRate));

    // 4 bytes for spell.
    bytedump spellBytes = m_Spell->dump();
    bytes.insert(bytes.end(), spellBytes.begin(), spellBytes.end());

    for (int i = 0; i < 6; i++) {
        bytes.push_back(static_cast<std::byte>(0));
    }

    bytes.push_back(static_cast<std::byte>(m_Pos.x));
    bytes.push_back(static_cast<std::byte>(m_Pos.y));

    return bytes;
}

SpellBuilding::SpellBuilding(bytedump bytes) : GenericBuilding(0) {
    assert(bytes.size() == 16);
    assert(static_cast<int>(bytes[0]) == SPELL_BUILDING_BYTE);

    m_HP = static_cast<int>(bytes[1]);
    m_TickCount = static_cast<int>(bytes[2]);
    m_AttackRate = static_cast<int>(bytes[3]);

    Positionable::m_Pos = {
        static_cast<int>(bytes[14]),
        static_cast<int>(bytes[15]),
    };

    bytedump spellBytes(bytes.begin() + 4, bytes.begin() + 8);
    m_Spell = dynamic_cast<TargettedSpell *>(Spell::fromByteDump(spellBytes));
}
