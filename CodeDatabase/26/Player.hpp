#pragma once
#include <stdexcept>
#include "Direction.hpp"

class Field;

class Player {
public:
	enum class CombatMode { Melee, Ranged };

	Player(int health, int meleeDamage, int rangedDamage);

	bool isAlive() const noexcept;
	int currentHealth() const noexcept;
	int meleeDamage() const noexcept;
	int rangedDamage() const noexcept;
	bool isMovementBlocked() const noexcept;
	void takeDamage(int dmg);
	int attackDamage() const noexcept;

	void switchCombatMode() noexcept;
	CombatMode mode() const noexcept;

	bool canMoveThisTurn() const noexcept;
	void applySlowForNextTurn() noexcept;
	bool consumeMovementAllowanceForThisTurn() noexcept;

	int kills() const noexcept;
	void addKill() noexcept;
	int maxHealth() const noexcept;

	void setRangedRange(int cells);
	void setCurrentHealth(int value);
	void setMaxHealth(int value);
	void setMeleeDamage(int value);
	void setRangedDamage(int value);
	void setKills(int value) noexcept;
	void setMode(CombatMode mode) noexcept;
	void setMovementBlocked(bool value) noexcept;
	int  rangedRange() const noexcept;

  	bool move   (Field& field, Direction dir);
    bool attack (Field& field, Direction dir);

private:
	int health_;
	int maxHealth_;   
	int meleeDamage_;
	int rangedDamage_;
	CombatMode mode_;
	bool movementBlockedThisTurn_;
	int kills_{0};
	int rangedRange_{3};
};
