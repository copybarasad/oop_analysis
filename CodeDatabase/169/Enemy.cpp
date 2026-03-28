#include "Enemy.hpp"

Enemy::Enemy(int hp, int damage)
	: hp(hp),
	  damage(damage) {}

Enemy::Enemy(const Enemy& other)
	: hp(other.hp), damage(other.damage) {}

Enemy::Enemy(Enemy&& other) noexcept
	: hp(other.hp), damage(other.damage) {
	other.hp = 0;
	other.damage = 0;
}

Enemy& Enemy::operator=(const Enemy& other) {
	if (this != &other) {
		hp = other.hp;
		damage = other.damage;
	}
	return *this;
}

Enemy& Enemy::operator=(Enemy&& other) noexcept {
	if (this != &other) {
		hp = other.hp;
		damage = other.damage;
		other.hp = 0;
		other.damage = 0;
	}
	return *this;
}

void Enemy::takeDamage(int amount) {
	if (amount <= 0) return;
	if (amount >= hp) hp = 0;
	else hp -= amount;
}

bool Enemy::isAlive() const noexcept {
	return hp > 0;
}

int Enemy::getDamage() const noexcept { 
	return damage; 
}
int Enemy::getHP() const noexcept { 
	return hp; 
}

