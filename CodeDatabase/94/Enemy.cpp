#include "Enemy.h"

Enemy::Enemy(int initialHealth, int enemyDamage) 
    : Character(initialHealth, 10) {
    
    setHealth(initialHealth);
    setMaxHealth(initialHealth);
}

Character* Enemy::clone() const {
    return new Enemy(*this);
}

void Enemy::performAction() {
}