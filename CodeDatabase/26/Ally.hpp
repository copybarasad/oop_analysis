#pragma once
#include "Position.hpp"

class Ally {
public:
	Ally(const Position& startPos, int health, int damage);
	~Ally();

	const Position& position() const noexcept;
	int health() const noexcept;
	int damage() const noexcept;

	bool isAlive() const noexcept;
	bool skipMovePending() const noexcept;

	void takeDamage(int dmg);

	void applySlowForNextTurn() noexcept;
	bool canMoveThisTurn() noexcept;

	void setPosition(const Position& p) noexcept;
	void setHealth(int value);
	void setSkipNextMove(bool value) noexcept;

private:
	Position pos_;
	int health_;
	int damage_;
	bool skipMoveNextTurn_;
};
