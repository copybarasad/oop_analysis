#pragma once
#include "Position.hpp"

class GameWorld;

class EnemyTower {
public:
	EnemyTower(const Position& position,
	           int attackRadius,
	           int damage,
	           int cooldownPeriod,
	           int health);
	~EnemyTower();

	const Position& position() const noexcept;
	bool isAlive() const noexcept;
	void takeDamage(int damage);

	void tickAndAttack(GameWorld& world);
	int attackRadius() const noexcept;
	int damage() const noexcept;
	int cooldownPeriod() const noexcept;
	int cooldownRemaining() const noexcept;
	int health() const noexcept;
	void setCooldownRemaining(int value) noexcept;
	void setHealth(int value);

private:
	Position position_;
	int attackRadius_;
	int damage_;
	int cooldownRemaining_;
	int cooldownPeriod_;
	int health_;
};
