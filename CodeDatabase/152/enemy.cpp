#include "enemy.h"

Enemy::Enemy(int health, int damage)
    : health_(health), damage_(damage) {
}

void Enemy::take_damage(int damage) {
    if (damage > 0) {
        health_ -= damage;
        if (health_ < 0) health_ = 0;
    }
}

void Enemy::set_position(int row, int col) {
    row_ = row;
    col_ = col;
}
