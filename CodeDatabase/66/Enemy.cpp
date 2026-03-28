#include "../include/Enemy.h"

Enemy::Enemy(int health, int damage, const std::vector<Direction>& patrolPattern)
    : health(health), damage(damage), patrolPattern(patrolPattern), currentPatrolStep(0) {}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

const std::vector<Direction>& Enemy::getPatrolPattern() const {
    return patrolPattern;
}

size_t Enemy::getCurrentPatrolStep() const {
    return currentPatrolStep;
}

Direction Enemy::getNextPatrolDirection() {
    if (patrolPattern.empty()) {
        return Direction::NONE;
    }
    
    Direction direction = patrolPattern[currentPatrolStep];
    currentPatrolStep = (currentPatrolStep + 1) % patrolPattern.size();
    return direction;
}

int Enemy::takeDamage(int damage) {
    int oldHealth = health;
    health -= damage;
    if (health < 0) {
        health = 0;
    }
    return oldHealth - health;
}

bool Enemy::isAlive() const {
    return health > 0;
}
