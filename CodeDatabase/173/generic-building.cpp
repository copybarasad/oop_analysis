#include <stdexcept>

#include "generic-building.hpp"

GenericBuilding::GenericBuilding(int hp) {
    if (hp < 0) {
        throw std::invalid_argument("hp must be positive");
    }

    m_HP = hp;
}

GenericBuilding::GenericBuilding(const GenericBuilding &building) {
    m_Pos = building.m_Pos;
}

Building *GenericBuilding::clone() { return new GenericBuilding(*this); }

void GenericBuilding::update() {}

void GenericBuilding::setMap(Map *) {}

void GenericBuilding::takeDamage(int damage) {
    if (damage < 0) {
        return;
    }

    m_HP -= std::min(damage, m_HP);
}

bool GenericBuilding::isAlive() { return m_HP != 0; }

std::string GenericBuilding::chr() { return "^"; }
Color GenericBuilding::color() { return Color::white(); }

bytedump GenericBuilding::dump() {
    bytedump bytes;
    bytes.reserve(16);

    bytes.push_back(static_cast<std::byte>(GENERIC_BUILDING_BYTE));
    bytes.push_back(static_cast<std::byte>(m_HP));
    for (int i = 0; i < 12; i++) {
        bytes.push_back(static_cast<std::byte>(0));
    }
    bytes.push_back(static_cast<std::byte>(m_Pos.x));
    bytes.push_back(static_cast<std::byte>(m_Pos.y));

    return bytes;
}

GenericBuilding::GenericBuilding(bytedump bytes) {
    assert(bytes.size() == 16);
    assert(static_cast<int>(bytes[0]) == GENERIC_BUILDING_BYTE);

    m_HP = static_cast<int>(bytes[1]);

    Positionable::m_Pos = {
        static_cast<int>(bytes[bytes.size() - 2]),
        static_cast<int>(bytes[bytes.size() - 1]),
    };
}
