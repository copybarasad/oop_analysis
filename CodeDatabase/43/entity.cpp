#include "entity.h"
#include <stdexcept>

Entity::Entity(int hp, int dmg, const std::string& entityType)
    : health(hp), maxHealth(hp), damage(dmg), type(entityType) {

    if (hp <= 0) throw std::invalid_argument("Health must be positive");
    if (dmg < 0) throw std::invalid_argument("Damage cannot be negative");
}

int Entity::getHealth() const { return health; }
int Entity::getMaxHealth() const { return maxHealth; }
int Entity::getDamage() const { return damage; }
const std::string& Entity::getType() const { return type; }
bool Entity::isAlive() const { return health > 0; }

void Entity::takeDamage(int amount) {
    if (amount < 0) throw std::invalid_argument("Damage amount cannot be negative");
    health -= amount;
    if (health < 0) health = 0;
}