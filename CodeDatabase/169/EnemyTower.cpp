#include "EnemyTower.hpp"

EnemyTower::EnemyTower(int hp, int damage, int range, int cooldown)
	: hp(hp), damage(damage), range(range), cooldown(cooldown), currentCooldown(0) {}

EnemyTower::EnemyTower(const EnemyTower& other)
	: hp(other.hp),
	  damage(other.damage),
	  range(other.range),
	  cooldown(other.cooldown),
	  currentCooldown(other.currentCooldown) {}

EnemyTower::EnemyTower(EnemyTower&& other) noexcept
	: hp(other.hp),
	  damage(other.damage),
	  range(other.range),
	  cooldown(other.cooldown),
	  currentCooldown(other.currentCooldown) {
	other.hp = 0;
	other.damage = 0;
	other.range = 0;
	other.cooldown = 0;
	other.currentCooldown = 0;
}

EnemyTower& EnemyTower::operator=(const EnemyTower& other) {
	if (this != &other) {
		hp = other.hp;
		damage = other.damage;
		range = other.range;
		cooldown = other.cooldown;
		currentCooldown = other.currentCooldown;
	}
	return *this;
}

EnemyTower& EnemyTower::operator=(EnemyTower&& other) noexcept {
	if (this != &other) {
		hp = other.hp;
		damage = other.damage;
		range = other.range;
		cooldown = other.cooldown;
		currentCooldown = other.currentCooldown;
		
		other.hp = 0;
		other.damage = 0;
		other.range = 0;
		other.cooldown = 0;
		other.currentCooldown = 0;
	}
	return *this;
}

int EnemyTower::getDamage() const noexcept {
	return damage;
}

int EnemyTower::getRange() const noexcept {
	return range;
}

bool EnemyTower::canAttack() const noexcept {
	return currentCooldown == 0 && isAlive();
}

int EnemyTower::attack() {
	if (!canAttack()) return 0;
	currentCooldown = cooldown;
	return damage;
}

void EnemyTower::update() {
	if (currentCooldown > 0) {
		--currentCooldown;
	}
}

void EnemyTower::takeDamage(int amount) {
	if (amount <= 0) return;
	if (amount >= hp) hp = 0;
	else hp -= amount;
}

bool EnemyTower::isAlive() const noexcept {
	return hp > 0;
}

int EnemyTower::getHP() const noexcept {
	return hp;
}

int EnemyTower::getCooldownRemaining() const noexcept {
	return currentCooldown;
}

void EnemyTower::setHP(int hp) {
	if (hp < 0) this->hp = 0;
	else this->hp = hp;
}

void EnemyTower::setCooldownRemaining(int cooldown) {
	if (cooldown < 0) currentCooldown = 0;
	else currentCooldown = cooldown;
}


