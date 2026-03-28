#include "Entity.h"

Entity::Entity(int startX, int startY, int health) 
    : hp(health), maxHP(health), x(startX), y(startY), alive(true) {
}

void Entity::takeDamage(int damage) {
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        alive = false;
    }
}

bool Entity::isAlive() const {
    return alive && hp > 0;
}

int Entity::getX() const {
    return x;
}

int Entity::getY() const {
    return y;
}

int Entity::getHP() const {
    return hp;
}

int Entity::getMaxHP() const {
    return maxHP;
}

void Entity::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Entity::setHP(int health) {
    hp = health;
    if (hp <= 0) {
        hp = 0;
        alive = false;
    } else if (hp > maxHP) {
        hp = maxHP;
    }
    if (hp > 0) {
        alive = true;
    }
}