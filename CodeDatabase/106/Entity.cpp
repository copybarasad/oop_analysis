#include "Entity.h"

Entity::Entity(const std::string& entityName, int startX, int startY, int health)
    : name(entityName), x(startX), y(startY), healthPoints(health), maxHealthPoints(health) {
    if (healthPoints <= 0) {
        healthPoints = 0;
    }
}

void Entity::move(int deltaX, int deltaY) {
    x += deltaX;
    y += deltaY;
}

void Entity::setPos(int nx, int ny) {
    x = nx; y = ny;
}

bool Entity::isAlive() const {
    return healthPoints > 0;
}

void Entity::takeDamage(int damage) {
    if (!isAlive()) return;
    healthPoints -= damage;
    if (healthPoints <= 0) healthPoints = 0;
}

void Entity::heal(int amount) {
    healthPoints += amount;
    if (healthPoints > maxHealthPoints) { healthPoints = maxHealthPoints; }
}


