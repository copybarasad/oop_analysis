#pragma once

class Enemy {
public:
	Enemy(int hp = 0, int damage = 0);
	Enemy(const Enemy& other);
	Enemy(Enemy&& other) noexcept;
	Enemy& operator=(const Enemy& other);
	Enemy& operator=(Enemy&& other) noexcept;
	~Enemy() = default;

	void takeDamage(int amount);
	bool isAlive() const noexcept;
	int getDamage() const noexcept;
	int getHP() const noexcept;

private:
	int hp;
	int damage;
};

