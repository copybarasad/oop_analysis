#include "game_object.h"
#include <stdexcept>
#include <fstream>

GameObject::GameObject(const Position& position, int health, int damage)
    : healthPoints(health),
    damagePoints(damage),
    currentPosition(position) {
}

int GameObject::getHealth() const {
    return healthPoints;
}

int GameObject::getDamage() const {
    return damagePoints;
}

const Position& GameObject::getPosition() const {
    return currentPosition;
}

void GameObject::setPosition(const Position& position) {
    currentPosition = position;
}

void GameObject::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }
    healthPoints -= damage;
    validateHealth();
}

bool GameObject::isAlive() const {
    return healthPoints > 0;
}

void GameObject::validateHealth() {
    if (healthPoints < 0) {
        healthPoints = 0;
    }
}

void GameObject::save(std::ostream& out) const {
    currentPosition.save(out);
    out.write(reinterpret_cast<const char*>(&healthPoints), sizeof(healthPoints));
    out.write(reinterpret_cast<const char*>(&damagePoints), sizeof(damagePoints));
}

void GameObject::load(std::istream& in) {
    currentPosition.load(in);
    in.read(reinterpret_cast<char*>(&healthPoints), sizeof(healthPoints));
    in.read(reinterpret_cast<char*>(&damagePoints), sizeof(damagePoints));
    validateHealth();
}