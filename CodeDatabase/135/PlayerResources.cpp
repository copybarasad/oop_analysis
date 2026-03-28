#include "PlayerResources.h"
#include <stdexcept>

PlayerResources::PlayerResources() : points(0), kills(0) {}

void PlayerResources::addPoints(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Cannot add negative points");
	}
	points += amount;
}

bool PlayerResources::spendPoints(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Cannot spend negative points");
	}
	if (points >= amount) {
		points -= amount;
		return true;
	}
	return false;
}

int PlayerResources::getPoints() const {
	return points;
}

void PlayerResources::setPoints(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Cannot set negative points");
	}
	points = amount;
}

void PlayerResources::incrementKills() {
	kills++;
}

int PlayerResources::getKills() const {
	return kills;
}

void PlayerResources::setKills(int amount) {
	if (amount < 0) {
		throw std::invalid_argument("Cannot set negative kills");
	}
	kills = amount;
}
