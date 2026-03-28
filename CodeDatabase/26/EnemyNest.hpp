#pragma once
#include <stdexcept>

class EnemyNest {
public:
	explicit EnemyNest(int spawnPeriod, int maxHealth = 20);

	int spawnPeriod() const noexcept;
	int maxHealth() const noexcept;

private:
	int spawnPeriod_;
	int maxHealth_;
};
