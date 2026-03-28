#include "Player.hpp"

Player::Player(int maxHP, int meleeDmg, int rangedDmg)
	: currentHP(maxHP),
	  maxHP(maxHP),
	  meleeDamage(meleeDmg),
	  rangedDamage(rangedDmg),
	  mode(AttackMode::Melee) {}

Player::Player(const Player& other)
	: currentHP(other.currentHP),
	  maxHP(other.maxHP),
	  meleeDamage(other.meleeDamage),
	  rangedDamage(other.rangedDamage),
	  mode(other.mode) {}

Player::Player(Player&& other) noexcept
	: currentHP(other.currentHP),
	  maxHP(other.maxHP),
	  meleeDamage(other.meleeDamage),
	  rangedDamage(other.rangedDamage),
	  mode(other.mode) {
	other.currentHP = 0;
	other.maxHP = 0;
	other.meleeDamage = 0;
	other.rangedDamage = 0;
	other.mode = AttackMode::Melee;
}

Player& Player::operator=(const Player& other) {
	if (this != &other) {
		currentHP = other.currentHP;
		maxHP = other.maxHP;
		meleeDamage = other.meleeDamage;
		rangedDamage = other.rangedDamage;
		mode = other.mode;
	}
	return *this;
}

Player& Player::operator=(Player&& other) noexcept {
	if (this != &other) {
		currentHP = other.currentHP;
		maxHP = other.maxHP;
		meleeDamage = other.meleeDamage;
		rangedDamage = other.rangedDamage;
		mode = other.mode;

		other.currentHP = 0;
		other.maxHP = 0;
		other.meleeDamage = 0;
		other.rangedDamage = 0;
		other.mode = AttackMode::Melee;
	}
	return *this;
}

void Player::takeDamage(int amount) {
	if (amount <= 0) return;
	if (amount >= currentHP) currentHP = 0;
	else currentHP -= amount;
}

void Player::setHP(int hp) {
	if (hp < 0) currentHP = 0;
	else if (hp > maxHP) currentHP = maxHP;
	else currentHP = hp;
}

bool Player::isAlive() const noexcept {
	return currentHP > 0;
}


int Player::currentDamage() const noexcept {
	if (mode == AttackMode::Melee) {
		return meleeDamage;
	} else {
		return rangedDamage;
	}
}

AttackMode Player::getMode() const noexcept {
	return mode;
}

void Player::toggleMode() {
	if (mode == AttackMode::Melee) {
		mode = AttackMode::Ranged;
	} else {
		mode = AttackMode::Melee;
	}
}

int Player::getHP() const noexcept { 
	return currentHP; 
}
int Player::getMaxHP() const noexcept { 
	return maxHP; 
}

