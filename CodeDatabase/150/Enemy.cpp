#include "Enemy.hpp"

Enemy::Enemy() : Character(0, 0, 30, 30, 15) {}

Enemy::Enemy(int x, int y, int health, int damage)
    : Character(x, y, health, health, damage) 
{}

void Enemy::serialize(std::ostream& os) const {
    Character::serialize(os);
}

void Enemy::deserialize(std::istream& is) {
    Character::deserialize(is);
}