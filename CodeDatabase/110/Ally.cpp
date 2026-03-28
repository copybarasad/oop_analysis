#include "Ally.hpp"

Ally::Ally (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue) :
    Entity(id, maxHealthValue, currentHealthValue, damageValue) {}

unsigned int Ally::calculateDamage () const { return damage; }