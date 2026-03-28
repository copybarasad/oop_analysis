#include "enemy.h"
#include "save_manager.h"

Enemy::Enemy(int startX, int startY, int health, int damage)
    : position(startX, startY), health(health), damage(damage) {}

Position Enemy::getPosition() const {
    return position;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

void Enemy::move(int deltaX, int deltaY) {
    position.x += deltaX;
    position.y += deltaY;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::save(std::ostream& os) const {
    os << position.x << " " << position.y << " " << health << " " << damage << "\n";
}

void Enemy::load(std::istream& is) {
    is >> position.x >> position.y >> health >> damage;
    if (is.fail()) throw SaveDataException("Failed to load enemy data");
    std::string leftover;
    std::getline(is, leftover);
}