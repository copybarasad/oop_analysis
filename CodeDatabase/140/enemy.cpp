#include "enemy.h"

Enemy::Enemy(int init_health, int attack_damage, int x, int y)
    : health(init_health),
      damage(attack_damage),
      position_x(x),
      position_y(y) {
        if (init_health <= 0) {
           init_health = 40;
        }
        if (attack_damage < 0) {
            attack_damage = 5;
        }
      }

void Enemy::take_damage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
}

void Enemy::set_position(int x, int y) {
    position_x = x;
    position_y = y;
}

bool Enemy::is_alive() const {
    return health > 0;
}

int Enemy::get_health() const {
    return health;
}

int Enemy::get_damage() const {
    return damage;
}

int Enemy::get_x() const {
    return position_x;
}

int Enemy::get_y() const {
    return position_y;
}
