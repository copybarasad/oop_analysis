#include "Enemy.h"

Enemy::Enemy(int hp, int dmg, const Position& pos)
    : Entity(pos, hp), damage(dmg) {}

int Enemy::GetDamage() const { return damage; }
void Enemy::SetDamage(int dmg) { damage = dmg; }
