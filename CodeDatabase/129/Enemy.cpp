#include "Enemy.h"

Enemy::Enemy() : Entity() {
    coords.x = ENEMY::E_X;
    coords.y = ENEMY::E_Y;
    damage = ENEMY::E_DAMAGE;
    lifes = ENEMY::E_LIFES; // у врага 2 жизни
}
Enemy::Enemy(int lifes) : Entity() {
    coords.x = ENEMY::E_X;
    coords.y = ENEMY::E_Y;
    damage = ENEMY::E_DAMAGE;
    this->lifes = lifes; 
}
Enemy::~Enemy() {}

bool Enemy::is_dead(){
    return is_alive();
}


Saving::EnemyData Enemy::serialize() const {
    return {coords.x, coords.y, lifes, damage};
}

void Enemy::deserialize(const Saving::EnemyData& data) {
    lifes = data.lifes;
    damage = data.damage;
    coords.x = data.x;
    coords.y = data.y;
}
