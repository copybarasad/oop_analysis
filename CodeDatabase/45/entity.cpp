#include "entity.hpp"

Entity::Entity(int health, int damageValue) 
    : hp(health), damage(damageValue) {
}

int Entity::getHp() const {
    return hp;
}

int Entity::getDamage() const {
    return damage;
}

void Entity::takeDamage(int damageAmount) {
    hp -= damageAmount;
    hp = hp > 0 ? hp : 0;
}

bool Entity::getCanMove() const {
    return canMove;
}

void Entity::setCanMove(bool can) {
    canMove = can;
}