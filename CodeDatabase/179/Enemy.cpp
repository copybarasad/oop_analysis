#include "Enemy.h"
#include <iostream>

Enemy::Enemy(int health, int damage, const std::string& name)
    : GameObject(health, name), damage(damage) {
    // Обработка отрицательных значений
    if (health < 0) {
        std::cerr << "Warning: Enemy health cannot be negative. Setting to 0.\n";
        setHealth(0);
    }
    if (damage < 0) {
        std::cerr << "Warning: Enemy damage cannot be negative. Setting to 0.\n";
        this->damage = 0;
    }
}

int Enemy::getDamage() const {
    return damage;
}

std::string Enemy::getType() const {
    return "Enemy";
}
