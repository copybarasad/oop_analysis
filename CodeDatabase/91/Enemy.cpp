#include "Enemy.h"
#include "SaveLoad.h"
#include <cmath>
#include <random>
#include <stdexcept>

Enemy::Enemy(const Position& startPosition, int enemyHealth, int enemyDamage)
  : health(enemyHealth),
    maxHealth(enemyHealth),
    damage(enemyDamage),
    position(startPosition)
{
    if (health <= 0) {
        throw std::invalid_argument("Enemy health must be positive");
    }
    if (damage < 0) {
        throw std::invalid_argument("Enemy damage cannot be negative");
    }
}

bool Enemy::moveTowards(const Position& targetPosition) {
    int deltaX = targetPosition.x - position.x;
    int deltaY = targetPosition.y - position.y;

    int moveX = 0, moveY = 0;

    if (std::abs(deltaX) > std::abs(deltaY)) {
        moveX = (deltaX > 0) ? 1 : -1;
    } else {
        moveY = (deltaY > 0) ? 1 : -1;
    }

    Position newPosition(position.x + moveX, position.y + moveY);

    if (newPosition == position) {
        return false;
    }

    position = newPosition;
    return true;
}

// === Enemy Fight System ===
int Enemy::getDamage() const {
    return damage;
}

void Enemy::takeDamage(int damage) {
    if (damage < 0) {
        throw std::invalid_argument("Damage cannot be negative");
    }

    health -= damage;

    if (health < 0) {
        health = 0;
    }
}

// === Getters ===
Position Enemy::getPosition() const {
    return position;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getMaxHealth() const {
    return maxHealth;
}

bool Enemy::isAlive() const {
    return health > 0;
}

// === Setters ===
void Enemy::setPosition(const Position& newPosition) {
    position = newPosition;
}

// === Setters for Save/Load ===
void Enemy::setHealth(int h) {
    if (h < 0)
        h = 0;
    if (h > maxHealth)
        h = maxHealth;
    health = h;
}

void Enemy::setMaxHealth(int h) {
    if (h <= 0)
        return;
    maxHealth = h;
    if (health > maxHealth)
        health = maxHealth;
}

void Enemy::setDamage(int d) {
    if (d < 0)
        return;
    damage = d;
}

void Enemy::loadState(const nlohmann::json& j) {
    setHealth(j.at("health").get<int>());
    setMaxHealth(j.at("maxHealth").get<int>());
    setDamage(j.at("damage").get<int>());
    setPosition(j.at("position").get<Position>());
}