#include "Attributes.h"
#include <stdexcept>

Attributes::Attributes(int intel, int dex, int str) 
	: intelligence(intel), dexterity(dex), strength(str) {
	if (intel < 0 || dex < 0 || str < 0) {
		throw std::invalid_argument("Характеристики не могут быть отрицательными");
	}
}

int Attributes::getIntelligence() const {
	return intelligence;
}

int Attributes::getDexterity() const {
	return dexterity;
}

int Attributes::getStrength() const {
	return strength;
}

void Attributes::increaseIntelligence(int amount) {
	if (amount > 0) {
		intelligence += amount;
	}
}

void Attributes::increaseDexterity(int amount) {
	if (amount > 0) {
		dexterity += amount;
	}
}

void Attributes::increaseStrength(int amount) {
	if (amount > 0) {
		strength += amount;
	}
}

int Attributes::calculateSpellPowerBonus() const {
	return intelligence / 10;
}

int Attributes::calculatePhysicalDamageBonus() const {
	return strength / 10;
}

int Attributes::calculateDodgeChance() const {
	int dodge = dexterity / 20;
	if (dodge > 50) {
		dodge = 50;
	}
	return dodge;
}

bool Attributes::canImprove() const {
	return intelligence < 100 || dexterity < 100 || strength < 100;
}
