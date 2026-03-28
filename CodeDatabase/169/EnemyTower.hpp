#pragma once
#include <memory>
#include "ISpell.hpp"

class EnemyTower {
public:
	EnemyTower(int hp = 60, int damage = 10, int range = 4, int cooldown = 2);
	EnemyTower(const EnemyTower& other);
	EnemyTower(EnemyTower&& other) noexcept;
	EnemyTower& operator=(const EnemyTower& other);
	EnemyTower& operator=(EnemyTower&& other) noexcept;
	~EnemyTower() = default;

	int getDamage() const noexcept;
	int getRange() const noexcept;
	bool canAttack() const noexcept;
	int attack();
	void update();
	void takeDamage(int amount);
	bool isAlive() const noexcept;
	int getHP() const noexcept;
	int getCooldownRemaining() const noexcept;
	
	void setHP(int hp);
	void setCooldownRemaining(int cooldown);

private:
	int hp;
	int damage;
	int range;
	int cooldown;
	int currentCooldown;
};


