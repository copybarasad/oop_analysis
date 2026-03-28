#include <stdexcept>

#include "../creature.hpp"
#include "damage-trap.hpp"

DamageTrap::DamageTrap(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("damage must be positive");
    }

    m_Damage = damage;
}

void DamageTrap::trigger(Creature *creature) { creature->takeDamage(m_Damage); }
