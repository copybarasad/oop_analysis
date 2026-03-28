#include "Enemy.h"

Enemy::Enemy()
    : Entity(100, 10) {}

Enemy::Enemy(int hp, int dmg)
    : Entity(hp, dmg) {}



Enemy::~Enemy() = default;

dataEnemy Enemy::Save(){
    dataEnemy data;
    data.damage=GetDamage();
    data.health=GetHealth();
    data.stunDuration=GetStunDuration();
    return data;
}