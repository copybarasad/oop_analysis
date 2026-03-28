#include "enemy.hpp"
#include "player.hpp"
#include <iostream>

Enemy::Enemy(int health, int damage)
    : Entity(health, damage) {}

void Enemy::Attack(Entity& target) {
    target.TakeDamage(getDamage());
}