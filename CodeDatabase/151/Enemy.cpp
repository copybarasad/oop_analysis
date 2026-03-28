#include "Enemy.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy(int startX, int startY, int health, int damage)
    : health(health), damage(damage), position(startX, startY) {
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

Position Enemy::getPosition() const {
    return position;
}

void Enemy::setPosition(const Position& newPos) {
    position = newPos;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}

bool Enemy::isAlive() const {
    return health > 0;
}

Position Enemy::getNextMove(const Position& playerPos) const {
    // Простой ИИ: двигается в сторону игрока
    Position nextPos = position;

    int dx = 0, dy = 0;

    // Случайное направление (50% шанс двигаться к игроку)
    if (rand() % 2 == 0) {
        if (position.x < playerPos.x) dx = 1;
        else if (position.x > playerPos.x) dx = -1;
    }
    else {
        if (position.y < playerPos.y) dy = 1;
        else if (position.y > playerPos.y) dy = -1;
    }

    // Если не двигаемся к игроку, двигаемся случайно
    if (dx == 0 && dy == 0) {
        switch (rand() % 4) {
        case 0: dx = 1; break;  // вправо
        case 1: dx = -1; break; // влево
        case 2: dy = 1; break;  // вниз
        case 3: dy = -1; break; // вверх
        }
    }

    nextPos.x += dx;
    nextPos.y += dy;

    return nextPos;
}