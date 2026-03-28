#include "EnemyTower.h"
#include "GameConstants.h"
#include <stdexcept>

EnemyTower::EnemyTower(int hp, int attack_damage)
	: tower_attack(attack_damage),
	  tower_health(hp),
	  cooldown_remaining(0) {
	if (hp <= 0) {
		throw std::invalid_argument("Enemy tower health must be positive");
	}
}

EntityType EnemyTower::getType() const {
	return EntityType::TOWER;
}

void EnemyTower::causeDamage(int damage) {
	tower_health.reduceCurrentHealth(damage);
}

int EnemyTower::getDamage() const {
	return tower_attack.getAttack();
}

std::pair<int, int> EnemyTower::getHealth() const {
	return {tower_health.getCurrentHealth(), tower_health.getMaxHealth()};
}

bool EnemyTower::alive() const {
	return tower_health.getCurrentHealth() > 0;
}

std::unique_ptr<IEntity> EnemyTower::clone() const {
	return std::make_unique<EnemyTower>(*this);
}

bool EnemyTower::canAttack() const {
	return cooldown_remaining == 0;
}

void EnemyTower::reduceCooldown() {
	if (cooldown_remaining > 0) {
		--cooldown_remaining;
	}
}

void EnemyTower::resetCooldown() {
	cooldown_remaining = max_cooldown;
}

int EnemyTower::getAttackRange() const {
	return GameConstants::TOWER_ATTACK_RANGE;
}
