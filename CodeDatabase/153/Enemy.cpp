#include "Enemy.h"
#include "GameConstants.h"

Enemy::Enemy(int startX, int startY)
    : Entity(GameConstants::ENEMY_START_HEALTH, GameConstants::ENEMY_DAMAGE),
    x(startX), y(startY) {
}

int Enemy::getX() const {
    return x;
}

int Enemy::getY() const {
    return y;
}

void Enemy::move(int newX, int newY) {
    x = newX;
    y = newY;
}

void Enemy::setHealth(int newHealth) {
    health = newHealth;
    if (health < 0) health = 0;
}