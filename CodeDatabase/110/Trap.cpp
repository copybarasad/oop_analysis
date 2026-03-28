#include "Trap.hpp"

Trap::Trap (size_t id, unsigned int damageValue, unsigned int maxHealthValue, unsigned int currentHealthValue) :
    Entity(id, maxHealthValue, currentHealthValue, damageValue) {}

unsigned int Trap::calculateDamage () const { return damage; }