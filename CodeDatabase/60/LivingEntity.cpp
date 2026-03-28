#include <LivingEntity.hpp>

LivingEntity::LivingEntity(int x, int y, Entity::Type type, int health, int expValue) : Entity(x, y, type), health(health), exp(expValue) {}

int LivingEntity::getHealth() const {return health;}

void LivingEntity::takeDamage(int amount) {
    if (health > 0) {
        health -= amount;
    }
}

void LivingEntity::setHealth(int value) {
    this->health = value;
}

void LivingEntity::setExp(int value) {
    exp = value;
}

int LivingEntity::getExp() const {
    return exp;
}