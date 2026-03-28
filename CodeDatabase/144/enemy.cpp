#include "enemy.h"
#include <algorithm>

Enemy::Enemy(int health, int damage, int x, int y) 
	: health(health), maxHealth(health), damage(damage), x(x), y(y) {}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getMaxHealth() const {
    return maxHealth;
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getX() const {
	return x;
}

int Enemy::getY() const {
	return y;
}

int Enemy::getPositionX() const {
	return x;
}

int Enemy::getPositionY() const {
	return y;
}

void Enemy::takeDamage(int damageAmount) {
	health = std::max(0, health - damageAmount);
}

void Enemy::setPosition(int newX, int newY) {
    x = newX;
    y = newY;
}

void Enemy::setHealth(int value) {
	health = std::max(0, value);
}

void Enemy::setMaxHealth(int value) {
	maxHealth = value;
}

void Enemy::setDamageValue(int value) {
	damage = value;
}

bool Enemy::isAlive() const {
	return health > 0;
}

void Enemy::onDefeated(EnemyActionContext& context) {
	context.rewardPlayer(10);
}

Enemy::Snapshot Enemy::createSnapshot() const {
	Snapshot snapshot;
	snapshot.health = health;
	snapshot.maxHealth = maxHealth;
	snapshot.damage = damage;
	snapshot.positionX = x;
	snapshot.positionY = y;
	return snapshot;
}

Enemy Enemy::fromSnapshot(const Snapshot& snapshot) {
	Enemy enemy(snapshot.maxHealth, snapshot.damage, snapshot.positionX, snapshot.positionY);
	enemy.setMaxHealth(snapshot.maxHealth);
	enemy.setHealth(snapshot.health);
	enemy.setDamageValue(snapshot.damage);
	return enemy;
}