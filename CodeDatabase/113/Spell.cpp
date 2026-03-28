#include "Spell.h"

Spell::Spell(const std::string& name, int manaCost, int range)
    : m_name(name), m_manaCost(manaCost), m_range(range) {}

const std::string& Spell::getName() const {
    return m_name;
}

int Spell::getManaCost() const {
    return m_manaCost;
}

int Spell::getRange() const {
    return m_range;
}