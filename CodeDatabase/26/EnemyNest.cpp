#include "EnemyNest.hpp"
#include <stdexcept>

EnemyNest::EnemyNest(int spawnPeriod, int maxHealth)
	: spawnPeriod_(spawnPeriod),
	  maxHealth_(maxHealth)
{
	if (spawnPeriod_ <= 0) {
		throw std::invalid_argument("EnemyNest: spawnPeriod must be > 0");
	}
	if (maxHealth_ <= 0) {
		throw std::invalid_argument("EnemyNest: maxHealth must be > 0");
	}
}

int EnemyNest::spawnPeriod() const noexcept {
	return spawnPeriod_;
}

int EnemyNest::maxHealth() const noexcept {
	return maxHealth_;
}
