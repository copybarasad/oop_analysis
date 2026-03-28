#include "generic-field.hpp"
#include "../creature.hpp"

GenericField::GenericField() {
    m_CreatureSlot = nullptr;
    m_BuildingSlot = nullptr;
    m_TrapSlot = nullptr;
}

GenericField::GenericField(const GenericField &field) {
    if (field.m_CreatureSlot != nullptr) {
        m_CreatureSlot = new Creature(*field.m_CreatureSlot);
    } else {
        m_CreatureSlot = nullptr;
    }

    if (field.m_BuildingSlot != nullptr) {
        m_BuildingSlot = field.m_BuildingSlot->clone();
    } else {
        m_BuildingSlot = nullptr;
    }
}

Field *GenericField::clone() { return new GenericField(*this); }

void GenericField::attack(Combattable *attacker) {
    if (m_CreatureSlot != nullptr) {
        attacker->dealDamage(m_CreatureSlot);
        return;
    }

    if (m_BuildingSlot != nullptr) {
        m_BuildingSlot->takeDamage(attacker->damage());
        return;
    }
}

void GenericField::damage(int damage) {
    if (m_CreatureSlot != nullptr) {
        m_CreatureSlot->takeDamage(damage);
        return;
    }

    if (m_BuildingSlot != nullptr) {
        m_BuildingSlot->takeDamage(damage);
        return;
    }
}

void GenericField::build(Building *building) { m_BuildingSlot = building; }

void GenericField::deoccupy() { m_CreatureSlot = nullptr; }

void GenericField::occupy(Creature *occupier) {
    m_CreatureSlot = occupier;

    if (m_TrapSlot != nullptr) {
        m_TrapSlot->trigger(m_CreatureSlot);
        m_TrapSlot = nullptr;
        return;
    }
}

void GenericField::placeTrap(Trap *trap) { m_TrapSlot = trap; }

bool GenericField::canAttack() {
    return m_CreatureSlot != nullptr || m_BuildingSlot != nullptr;
}

bool GenericField::canBuild() {
    return m_CreatureSlot == nullptr && m_BuildingSlot == nullptr &&
           m_TrapSlot == nullptr;
}

bool GenericField::canOccupy() {
    return m_CreatureSlot == nullptr && m_BuildingSlot == nullptr;
}

bool GenericField::canPlaceTrap() {
    return m_CreatureSlot == nullptr && m_BuildingSlot == nullptr &&
           m_TrapSlot == nullptr;
}

void GenericField::update() {
    if (m_CreatureSlot != nullptr && !m_CreatureSlot->isAlive()) {
        m_CreatureSlot = nullptr;
    }

    if (m_BuildingSlot != nullptr && !m_BuildingSlot->isAlive()) {
        m_BuildingSlot = nullptr;
    }
}

bool GenericField::isOccupiedByEnemy() {
    return m_CreatureSlot != nullptr && m_CreatureSlot->isEnemy();
}

std::string GenericField::chr() {
    if (m_CreatureSlot != nullptr) {
        return m_CreatureSlot->chr();
    }

    if (m_BuildingSlot != nullptr) {
        return m_BuildingSlot->chr();
    }

    return ".";
}

Color GenericField::color() {
    if (m_CreatureSlot != nullptr) {
        return m_CreatureSlot->color();
    }

    if (m_BuildingSlot != nullptr) {
        return m_BuildingSlot->color();
    }

    return Color::white();
};

bytedump GenericField::dump() {
    bytedump bytes;
    bytes.push_back(static_cast<std::byte>(GENERIC_FIELD_BYTE));
    return bytes;
}
