#pragma once
#include "Enums.hpp"

class Player {
public:
	Player(int maxHP = 0, int meleeDmg = 0, int rangedDmg = 0);
	Player(const Player& other);
	Player(Player&& other) noexcept;
	Player& operator=(const Player& other);
	Player& operator=(Player&& other) noexcept;
	~Player() = default;

	void takeDamage(int amount);
	void setHP(int hp);
	bool isAlive() const noexcept;
	int currentDamage() const noexcept;
	AttackMode getMode() const noexcept;
	void toggleMode();
	int getHP() const noexcept;
	int getMaxHP() const noexcept;

private:
	int currentHP;
	int maxHP;
	int meleeDamage;
	int rangedDamage;
	AttackMode mode;
};

