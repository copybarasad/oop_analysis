#include "Enemy.h"

Enemy::Enemy(int x,int y,int Health, int Damage) 
    : Character(x,y,Health),
      damage(Damage) {}

int Enemy::getDamage() const {
    return damage;
}