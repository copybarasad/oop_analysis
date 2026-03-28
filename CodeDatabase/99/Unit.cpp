#include "../h/Unit.h"

using namespace std;

Unit::Unit(int hp, int damage, int score) 
	: hp(hp), damage(damage), score(score) {
}

int Unit::getHP() const {
	return hp;
}

int Unit::getDamage() const {
	return damage;
}

int Unit::getScore() const {
	return score;
}

// Applies damage to the unit, ensuring HP never goes below zero
// This maintains the invariant: HP >= 0
void Unit::takeDamage(int amount) {
	hp -= amount;
	if (hp < 0) {
		hp = 0;
	}
}

// Adds points to the unit's score
// Used when player defeats enemies
void Unit::addScore(int points) {
	score += points;
}

// Checks if the unit is still alive (HP > 0)
bool Unit::isAlive() const {
	return hp > 0;
}
