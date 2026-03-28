#include "entity.hpp"
#include "field/field.hpp"
#include "game/game.hpp"

Entity::Entity(Field &field, Point position, int health)
    : field(field), position(position), health(health), hp(health) {
    field[position].set_entity(this);
}

void Entity::take_damage(int damage) {
    this->health -= damage;
    if (health <= 0) {
        is_alive = false;
        field[position].set_entity(nullptr);
    }
}

bool Entity::is_entity_alive() const {
    return is_alive;
}

int Entity::get_health() const {
    return health;
}

int Entity::get_max_hp() const {
    return hp;
}

Team Entity::get_team() const {
    return Team::Neutral;
}

Point Entity::get_position() const {
    return this->position;
}

void Entity::set_position(Point pos) {
    field[position].set_entity(nullptr);
    position = pos;
    field[position].set_entity(this);
}

void Entity::restore_health(int amount) {
    health = std::min(hp, health + amount);
}

void Entity::set_health(int new_health) {
    health = std::min(hp, new_health);
}

Entity* Entity::get_pending_entity() {
    return nullptr;
}