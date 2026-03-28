#include "Enemy.h"
#include <cstdlib>
#include <ctime>

Enemy::Enemy(int health, int damage) 
    : health(health), damage(damage), pos(0, 0), alive(true) {}

int Enemy::getHealth() const { return health; }
int Enemy::getDamage() const { return damage; }
const Position& Enemy::getPos() const { return pos; }
bool Enemy::isAlive() const { return alive; }

void Enemy::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
        checkAlive();
    }
}

void Enemy::move(int dx, int dy) {
    pos.move(dx, dy);
}

void Enemy::setPos(int x, int y) {
    pos.set(x, y);
}

void Enemy::checkAlive() {
    alive = (health > 0);
}

void Enemy::makeMove(const Position& playerPos, int width, int height) {
    if (!alive) return;
    
    int direction = rand() % 4;
    int newX = pos.getX();
    int newY = pos.getY();
    
    switch (direction) {
        case 0: if (newY > 0) newY--; break;
        case 1: if (newY < height - 1) newY++; break;
        case 2: if (newX > 0) newX--; break;
        case 3: if (newX < width - 1) newX++; break;
    }

    Position newPos(newX, newY);
    if (newPos == playerPos) {
        return;
    }
    
    setPos(newX, newY);
}