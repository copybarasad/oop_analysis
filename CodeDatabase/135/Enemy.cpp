#include "Enemy.h"
#include <stdexcept>

Enemy::Enemy(int attack, int hp)
	: enemy_attack(attack), enemy_health(hp) {
	if (attack < 0) {
		throw std::invalid_argument("Enemy attack cannot be negative");
	}
	if (hp <= 0) {
		throw std::invalid_argument("Enemy health must be positive");
	}
}

EntityType Enemy::getType() const {
	return EntityType::ENEMY;
}

void Enemy::causeDamage(int damage) {
	enemy_health.reduceCurrentHealth(damage);
}

int Enemy::getDamage() const {
	return enemy_attack.getAttack();
}

std::pair<int, int> Enemy::getHealth() const {
	return {enemy_health.getCurrentHealth(), enemy_health.getMaxHealth()};
}

bool Enemy::alive() const {
	return enemy_health.getCurrentHealth() > 0;
}

std::unique_ptr<IEntity> Enemy::clone() const {
	return std::make_unique<Enemy>(*this);
}
