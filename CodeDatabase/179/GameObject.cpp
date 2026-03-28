#include "GameObject.h"

GameObject::GameObject(int health, const std::string& name)
    : health(health), maxHealth(health), name(name) {}

int GameObject::getHealth() const {
    return health;
}

int GameObject::getMaxHealth() const {
    return maxHealth;
}

std::string GameObject::getName() const {
    return name;
}

void GameObject::setHealth(int newHealth) {
    health = newHealth;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

void GameObject::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool GameObject::isAlive() const {
    return health > 0;
}
