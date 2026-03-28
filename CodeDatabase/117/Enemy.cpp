#include "Enemy.h"

Enemy::Enemy(int maxHealth, int damage, CharacterCondition condition): Character(maxHealth, condition), damage(damage) {}

Enemy::Enemy(): Enemy(DEFAULT_MAX_HEALTH, DEFAULT_DAMAGE, CharacterCondition::Normal) {}

int Enemy::getDMG() const{
    return this->damage;
}

void Enemy::setDMG(int damage){
    this->damage = damage;
}

std::string Enemy::getName(){
    return "Enemy";
}