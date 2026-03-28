#include "Ally.h"
#include <memory>

Ally::Ally(int attack, int hp) 
	: ally_attack(attack), ally_health(hp) {}

EntityType Ally::getType() const {
	return EntityType::ALLY;
}

void Ally::causeDamage(int damage) {
	ally_health.reduceCurrentHealth(damage);
}

int Ally::getDamage() const {
	return ally_attack.getAttack();
}

std::pair<int, int> Ally::getHealth() const {
	return {ally_health.getCurrentHealth(), ally_health.getMaxHealth()};
}

bool Ally::alive() const {
	return ally_health.getCurrentHealth() > 0;
}

std::unique_ptr<IEntity> Ally::clone() const {
	return std::make_unique<Ally>(*this);
}
