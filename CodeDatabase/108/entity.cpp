#include "entity.h"

#include <utility>

Entity::Entity(std::string name, const Position& position, int health, int damage) :
    name(std::move(name)), position(position), max_health(health), health(health), damage(damage) {}


void Entity::take_damage(int damage_value) {
    health -= damage_value;
    if (health < 0) health = 0;
}

bool Entity::is_alive() const {
    return health > 0;
}

Position Entity::get_position() const {
    return position;
}

int Entity::get_health() const {
    return health;
}

int Entity::get_damage() const {
    return damage;
}

std::string Entity::get_name() const {
    return name;
}



void Entity::set_position(const Position& new_position) {
    position = new_position;
}


void Entity::set_max_health(const int new_max_health) {
    max_health = new_max_health;
}

void Entity::set_health(const int new_health) {
    health = new_health;
}

int Entity::get_max_health() const {
    return max_health;
}












