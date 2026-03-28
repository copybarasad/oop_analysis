#include "EnemyBarracks.h"
#include <stdexcept>

EnemyBarracks::EnemyBarracks(int hp, int period)
	: barracks_health(hp), spawn_period(period), counter(period) {
	if (hp <= 0) {
		throw std::invalid_argument("Barracks health must be positive");
	}
	if (period <= 0) {
		throw std::invalid_argument("Spawn period must be positive");
	}
}

EntityType EnemyBarracks::getType() const {
	return EntityType::BARRACKS;
}

void EnemyBarracks::causeDamage(int damage) {
	barracks_health.reduceCurrentHealth(damage);
}

std::pair<int, int> EnemyBarracks::getHealth() const {
	return {barracks_health.getCurrentHealth(), barracks_health.getMaxHealth()};
}

bool EnemyBarracks::alive() const {
	return barracks_health.getCurrentHealth() > 0;
}

int EnemyBarracks::timeToSpawn() const {
	return counter;
}

std::unique_ptr<IEntity> EnemyBarracks::clone() const {
	return std::make_unique<EnemyBarracks>(*this);
}

void EnemyBarracks::decrementCounter() {
	if (counter > 0) {
		counter--;
	}
}

bool EnemyBarracks::readyToSpawn() const {
	return counter <= 0;
}

void EnemyBarracks::resetCounter() {
	counter = spawn_period;
}

bool EnemyBarracks::update() {
	decrementCounter();
	if (readyToSpawn()) {
		resetCounter();
		return true;
	}
	return false;
}
