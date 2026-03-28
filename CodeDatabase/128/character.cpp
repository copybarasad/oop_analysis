#include "entity.hpp"
#include <algorithm>

Entity::Entity(int health, int damage)
    : health_(health), damage_(damage) {
    if (health <= 0)
        throw std::invalid_argument("Entity: health must be > 0!");
    if (damage < 0)
        throw std::invalid_argument("Entity: damage must be >= 0!");
}

void Entity::TakeDamage(int amount) {
    if (amount < 0)
        throw std::invalid_argument("Entityr: amount must be >= 0!");
    if (health_ == 0)
        return;
    health_ = std::max(0, health_ - amount);
}

void Entity::Attack(Entity& target) {
    target.TakeDamage(getDamage());
}
