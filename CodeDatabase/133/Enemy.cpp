#include "Enemy.h"
Enemy::Enemy(int enemyHealth, int enemyDamage) : health(enemyHealth), damage(enemyDamage) {
    if (health < 0) health = GameConstants::DEFAULT_ENEMY_HEALTH;
    if (damage < 0) damage = GameConstants::DEFAULT_ENEMY_DAMAGE;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

void Enemy::takeDamage(int damageTaken) {
    health -= damageTaken;
    if (health < 0) health = 0;
}

bool Enemy::isAlive() const {
    return health > 0;
}