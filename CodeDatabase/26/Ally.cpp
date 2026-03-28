#include "Ally.hpp"
#include <stdexcept>

Ally::Ally(const Position& startPos, int health, int damage)
	: pos_(startPos),
	  health_(health),
	  damage_(damage),
	  skipMoveNextTurn_(false)
{
	if (health_ <= 0) {
		throw std::invalid_argument("Ally: non-positive health");
	}
	if (damage_ < 0) {
		throw std::invalid_argument("Ally: negative damage");
	}
}

Ally::~Ally() {}

const Position& Ally::position() const noexcept {
	return pos_;
}

int Ally::health() const noexcept {
	return health_;
}

int Ally::damage() const noexcept {
	return damage_;
}

bool Ally::isAlive() const noexcept {
	return health_ > 0;
}

bool Ally::skipMovePending() const noexcept {
	return skipMoveNextTurn_;
}

void Ally::takeDamage(int dmg) {
	if (dmg < 0) {
		throw std::invalid_argument("Ally::takeDamage: negative dmg");
	}
	if (!isAlive()) {
		return;
	}
	if (dmg >= health_) {
		health_ = 0;
	} else {
		health_ -= dmg;
	}
}

void Ally::applySlowForNextTurn() noexcept {
	skipMoveNextTurn_ = true;
}

bool Ally::canMoveThisTurn() noexcept {
	if (skipMoveNextTurn_) {
		skipMoveNextTurn_ = false;
		return false;
	}
	return true;
}

void Ally::setPosition(const Position& p) noexcept {
	pos_ = p;
}

void Ally::setHealth(int value) {
	if (value < 0) {
		throw std::invalid_argument("Ally::setHealth: value < 0");
	}
	health_ = value;
}

void Ally::setSkipNextMove(bool value) noexcept {
	skipMoveNextTurn_ = value;
}
