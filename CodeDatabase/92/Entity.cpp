#include "Entity.h"

Entity::Entity(std::pair<int, int> position, float health, int maxHealth, int damage)
    : health(health), maxHealth(maxHealth), damage(damage), position(position) {}

float Entity::attack() {
    return damage;
}

std::pair<int, int> Entity::getPosition() const {
    return position;
}
void Entity::setPosition(std::pair<int, int> pos) {
    position = pos;
}

json Entity::to_json() {
    return json{
            {"x", position.first},
            {"y", position.second},
            {"health", health},
            {"maxHealth", maxHealth},
            {"damage", damage}
    };
}

bool Entity::from_json(json& j) {
    try {
        position.first = j.at("x");
        position.second = j.at("y");
        health = j.at("health");
        maxHealth = j.at("maxHealth");
        damage = j.at("damage");
        return true;
    } catch(...) { return false; }
}
