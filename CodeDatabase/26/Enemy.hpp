#pragma once
#include <stdexcept>
class Enemy {
public:
	Enemy(int health, int damage);
	bool isAlive() const noexcept;
	void takeDamage(int dmg);
	int damage() const noexcept;
	int health() const noexcept;
	void setHealth(int value);

private:
	int health_;
	int damage_;
};
