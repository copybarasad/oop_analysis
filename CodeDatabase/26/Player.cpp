#include "Field.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Direction.hpp"
#include <iostream>

extern bool g_verbose;

Player::Player(int health, int meleeDamage, int rangedDamage)
	: health_(health),
	maxHealth_(health),
	meleeDamage_(meleeDamage),
	rangedDamage_(rangedDamage),
	mode_(CombatMode::Melee),
	movementBlockedThisTurn_(false)
{
	if (health_ <= 0) throw std::invalid_argument("Player: health must be > 0");
	if (meleeDamage_ < 0 || rangedDamage_ < 0) throw std::invalid_argument("Player: damage must be >= 0");
}

bool Player::isAlive() const noexcept {
	return health_ > 0;
}

int Player::currentHealth() const noexcept {
	return health_;
}

int Player::meleeDamage() const noexcept {
	return meleeDamage_;
}

int Player::rangedDamage() const noexcept {
	return rangedDamage_;
}

int  Player::maxHealth() const noexcept { return maxHealth_; } 

void Player::takeDamage(int dmg) {
	if (dmg < 0) throw std::invalid_argument("takeDamage: dmg < 0");
	health_ -= dmg;
	if (health_ < 0) health_ = 0;
}

int Player::attackDamage() const noexcept {
	return (mode_ == CombatMode::Melee) ? meleeDamage_ : rangedDamage_;
}

void Player::switchCombatMode() noexcept {
	mode_ = (mode_ == CombatMode::Melee) ? CombatMode::Ranged : CombatMode::Melee;
}

Player::CombatMode Player::mode() const noexcept {
	return mode_;
}

bool Player::canMoveThisTurn() const noexcept {
	return !movementBlockedThisTurn_;
}

bool Player::isMovementBlocked() const noexcept {
	return !movementBlockedThisTurn_;
}

void Player::applySlowForNextTurn() noexcept {
	movementBlockedThisTurn_ = true;
}

bool Player::consumeMovementAllowanceForThisTurn() noexcept {
	if (movementBlockedThisTurn_) {
		movementBlockedThisTurn_ = false;
		return false;
	}
	return true;
} 

int Player::kills() const noexcept { return kills_; }

void Player::addKill() noexcept { ++kills_; }

void Player::setRangedRange(int cells) {
	if (cells <= 0) throw std::invalid_argument("Player::setRangedRange: cells must be > 0");
	rangedRange_ = cells;
}

int Player::rangedRange() const noexcept {
	return rangedRange_;
}

void Player::setCurrentHealth(int value) {
	if (value < 0 || value > maxHealth_) {
		throw std::invalid_argument("Player::setCurrentHealth: value out of range");
	}
	health_ = value;
}

void Player::setMaxHealth(int value) {
	if (value <= 0) {
		throw std::invalid_argument("Player::setMaxHealth: value must be > 0");
	}
	maxHealth_ = value;
	if (health_ > maxHealth_) {
		health_ = maxHealth_;
	}
}

void Player::setMeleeDamage(int value) {
	if (value < 0) {
		throw std::invalid_argument("Player::setMeleeDamage: value must be >= 0");
	}
	meleeDamage_ = value;
}

void Player::setRangedDamage(int value) {
	if (value < 0) {
		throw std::invalid_argument("Player::setRangedDamage: value must be >= 0");
	}
	rangedDamage_ = value;
}

void Player::setKills(int value) noexcept {
	if (value < 0) value = 0;
	kills_ = value;
}

void Player::setMode(CombatMode mode) noexcept {
	mode_ = mode;
}

void Player::setMovementBlocked(bool value) noexcept {
	movementBlockedThisTurn_ = value;
}

bool Player::move(Field& field, Direction dir) {
    if (!field.playerPos_.has_value() || !isAlive()) return false;
    if (!consumeMovementAllowanceForThisTurn()) return false;

    Position from = *field.playerPos_;
    Position to = field.stepOf(dir, from);

    if (!field.isInside(to)) return false;
    if (!field.cellIsPassable(to)) return false;
    if (field.findEnemyAt(to) != -1) return false;
    if (field.hasAllyAt(to)) return false;
    if (field.hasTowerAt(to)) return false;
    if (field.hasNestAt(to)) return false;

    field.playerPos_ = to;

    if (field.at(*field.playerPos_).isSlow()) {
        applySlowForNextTurn();
    }
    return true;
}

bool Player::attack(Field& field, Direction dir) {
    if (!field.playerPos_.has_value() || !isAlive()) return false;

    const int dmg = attackDamage();
    if (dmg <= 0) return false;

    Position p = *field.playerPos_;
    if (mode() == CombatMode::Melee) {
        Position target = field.stepOf(dir, p);
        if (!field.isInside(target)) return false;
        if (field.dealDamageToEnemyAt(target, dmg)) {
            if (g_verbose) std::cout << "Player attacks enemy at (" << target.x << "," << target.y << ") dmg=" << dmg << "\n";
            return true;
        }
        return false;
    }

    Position cur = p;
    for (int step = 0; step < rangedRange(); ++step) {
        cur = field.stepOf(dir, cur);
        if (!field.isInside(cur)) break;
        if (!field.cellIsPassable(cur)) break;
        if (field.dealDamageToEnemyAt(cur, dmg)) {
            if (g_verbose) std::cout << "Player ranged hits enemy at (" << cur.x << "," << cur.y << ") dmg=" << dmg << "\n";
            return true;
        }
    }
    return false;
}


