#include "Cell.h"
#include <string>

Cell::Cell(bool isPassable)
    : m_isPassable(isPassable), m_hasTrap(false), m_trapDamage(0), m_trapType("") {}

bool Cell::isPassable() const {
    return m_isPassable;
}

void Cell::setPassable(bool passable) {
    m_isPassable = passable;
}

bool Cell::hasTrap() const {
    return m_hasTrap;
}

void Cell::setTrap(int damage) {
    m_hasTrap = true;
    m_trapDamage = damage;
    m_trapType = "Basic";
}

void Cell::setTrap(const ITrapSpell& trapSpell) {
    m_hasTrap = true;
    m_trapDamage = trapSpell.getTrapDamage();
    m_trapType = trapSpell.getTrapType();
}

void Cell::removeTrap() {
    m_hasTrap = false;
    m_trapDamage = 0;
    m_trapType = "";
}

int Cell::getTrapDamage() const {
    return m_trapDamage;
}

std::string Cell::getTrapType() const {
    return m_trapType;
}