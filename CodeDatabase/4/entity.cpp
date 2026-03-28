#include "entity.hpp"

Entity::Entity(int id, int health, int damage)
    : id(id), health(health), damage(damage) {}

bool Entity::alive() const {
    return health > 0;
}

void Entity::get_damage(int dmg) {
    health -= dmg;
}