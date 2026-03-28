#include "Ally.h"

Ally::Ally(int allyHealth, int allyDamage)
	: health(allyHealth), damage(allyDamage), lifetime(10) {
	if (health < 0) health = GameConstants::DEFAULT_ALLY_HEALTH;
	if (damage < 0) damage = GameConstants::DEFAULT_ALLY_DAMAGE;
}

int Ally::getHealth() const {
	return health;
}

int Ally::getDamage() const {
	return damage;
}

int Ally::getLifetime() const {
	return lifetime;
}

void Ally::setLifetime(int value) {
	lifetime = value;
	if (lifetime < 0) {
		lifetime = 0;
	}
}

void Ally::takeDamage(int damageTaken) {
	health -= damageTaken;
	if (health < 0) health = 0;
}

bool Ally::isAlive() const {
	return health > 0;
}

bool Ally::shouldDespawn() const {
	return lifetime <= 0;
}

void Ally::updateLifetime() {
	lifetime--;
}