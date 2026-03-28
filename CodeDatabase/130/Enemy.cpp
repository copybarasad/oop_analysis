#include "Enemy.h"

Enemy::Enemy(int hp, int dmg)
    : health(hp), maxHealth(hp), damage(dmg), alive(true), slowed(false), skipNextTurn(false), position{-1, -1} {}

int Enemy::getHealth() const { return health; }
int Enemy::getMaxHealth() const { return maxHealth; }
int Enemy::getDamage() const { return damage; }
bool Enemy::isAlive() const { return alive; }
Pos Enemy::getPosition() const { return position; }

void Enemy::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0)
        die();
}

void Enemy::die() { alive = false; }

void Enemy::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

void Enemy::setPosition(const Pos& pos) {
    position = pos;
}

Direction Enemy::getRandomDirection() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);
    return static_cast<Direction>(dist(gen));
}

bool Enemy::isSlowed() const { return slowed; }
void Enemy::setSlowed(bool value) { slowed = value; }

bool Enemy::shouldSkipTurn() const { return skipNextTurn; }
void Enemy::skipNextMove() { skipNextTurn = true; }
void Enemy::resetSkipTurn() { skipNextTurn = false; }