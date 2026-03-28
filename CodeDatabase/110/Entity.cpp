#include "Entity.hpp"

Entity::Entity (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue, bool skipNextTurn) :
    id(id), maxHealth(maxHealthValue), currentHealth(currentHealthValue), damage(damageValue), skipNextTurn(skipNextTurn) {}

void Entity::setID (size_t id) { this->id = id; }

void Entity::setMaxHealth (unsigned int maxHealthValue) {
    currentHealth = std::min(currentHealth, maxHealthValue);
    maxHealth = maxHealthValue;
}

void Entity::setCurrentHealth (unsigned int currentHealthValue) {
    maxHealth = std::max(maxHealth, currentHealthValue);
    currentHealth = currentHealthValue;
}

void Entity::setDamage (unsigned int damageValue) { damage = damageValue; }

void Entity::setSkip () { skipNextTurn = 1; }

void Entity::restoreSkip () { skipNextTurn = 0; }

size_t Entity::getID () const { return id; }

unsigned int Entity::getMaxHealth () const { return maxHealth; }

unsigned int Entity::getCurrentHealth () const { return currentHealth; }

bool Entity::isSkipping () const { return skipNextTurn; }