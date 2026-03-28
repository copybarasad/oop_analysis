#include "enemy.hpp"

Enemy::Enemy(int temp_level, int number)
    : Entity(number, 100 + 100 * (temp_level - 1), temp_level * 15)
    , skip_turn(false) {}

Enemy::Enemy(Enemy&& other) noexcept
    : Entity(other.id, other.health, other.damage)
    , skip_turn(other.skip_turn) {}

Enemy& Enemy::operator=(Enemy&& other) noexcept {
    if (this != &other) {
        id = other.id;
        health = other.health;
        damage = other.damage;
        skip_turn = other.skip_turn;
    }
    return *this;
}