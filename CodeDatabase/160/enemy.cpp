#include "../include/entity/enemy.h"

Enemy::Enemy() : Entity(50), damage_(5) {}

Enemy::Enemy(int max_health, int damage) : Entity(max_health), damage_(damage) {}

int Enemy::damage() const { 
    return this->damage_; 
}