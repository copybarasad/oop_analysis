#include "Attack.h"
#include <stdexcept>

Attack::Attack(int attack_value) : attack(attack_value) {
	if (attack_value < 0) {
		throw std::invalid_argument("Attack value cannot be negative");
	}
}

bool Attack::setAttack(int new_attack) {
	if (new_attack < 0) {
		return false;
	}
	attack = new_attack;
	return true;
}

int Attack::getAttack() const {
	return attack;
}

int Attack::calculateTotalDamage(int bonus) const {
	int total = attack + bonus;
	return total > 0 ? total : 0;
}
