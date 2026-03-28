#include "Enemy.hpp"

Enemy::Enemy(int health, int damage)
	: health_(health), damage_(damage)
{
	if (health_ <= 0) throw std::invalid_argument("Enemy: health must be > 0");
	if (damage_ < 0) throw std::invalid_argument("Enemy: damage must be >= 0");
}

bool Enemy::isAlive() const noexcept {
	return health_ > 0;
}

void Enemy::takeDamage(int dmg) {
	if (dmg < 0) throw std::invalid_argument("Enemy::takeDamage: dmg < 0");
	health_ -= dmg;
	if (health_ < 0) health_ = 0;
}

int Enemy::damage() const noexcept {
	return damage_;
}

int Enemy::health() const noexcept {
	return health_;
}

void Enemy::setHealth(int value) {
	if (value < 0) {
		throw std::invalid_argument("Enemy::setHealth: value < 0");
	}
	health_ = value;
}
