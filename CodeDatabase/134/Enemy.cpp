#include "Enemy.h"

Enemy::Enemy(int maxHealth, int damage):
    health(maxHealth),
    maxHealth(maxHealth),
    attackDamage(damage),
    posX(0),
    posY(0)
{    
}

bool Enemy::IsAlive() const {
    return health > 0;
}

void Enemy::Move(int dx, int dy) {
    posX += dx;
    posY += dy;
}

void Enemy::TakeDamage(int damage_amount) {
    health -= damage_amount;
    if (health < 0){
        health = 0;
    }
}

int Enemy::GetX() const {
    return posX;
}

int Enemy::GetY() const {
    return posY;
}

std::pair<int, int> Enemy::GetPos() const {
    return {posX, posY};
}

int Enemy::GetHealth() const {
    return health;
}

int Enemy::GetDamage() const {
    return attackDamage;
}

