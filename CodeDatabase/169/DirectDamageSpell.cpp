#include "DirectDamageSpell.hpp"
#include "SpellManager.hpp"

DirectDamageSpell::DirectDamageSpell(int damage, int range)
	: damage(damage), range(range) {}

DirectDamageSpell::DirectDamageSpell(const DirectDamageSpell& other)
	: damage(other.damage), range(other.range) {}

DirectDamageSpell::DirectDamageSpell(DirectDamageSpell&& other) noexcept
	: damage(other.damage), range(other.range) {
	other.damage = 0;
	other.range = 0;
}

DirectDamageSpell& DirectDamageSpell::operator=(const DirectDamageSpell& other) {
	if (this != &other) {
		damage = other.damage;
		range = other.range;
	}
	return *this;
}

DirectDamageSpell& DirectDamageSpell::operator=(DirectDamageSpell&& other) noexcept {
	if (this != &other) {
		damage = other.damage;
		range = other.range;
		other.damage = 0;
		other.range = 0;
	}
	return *this;
}

bool DirectDamageSpell::cast(SpellManager& spellManager, const Coords& casterPos, const Coords& target) {
	if (spellManager.manhattan(casterPos, target) > range) {
		return false;
	}
	
	return spellManager.dealDamageAt(target, damage);
}

int DirectDamageSpell::getRange() const noexcept {
	return range;
}

int DirectDamageSpell::getDamage() const noexcept {
	return damage;
}

std::string DirectDamageSpell::getName() const {
	return "Прямой урон";
}

std::string DirectDamageSpell::getDescription() const {
	return "Наносит " + std::to_string(damage) + " урона врагу или зданию в радиусе " + std::to_string(range);
}

std::unique_ptr<ISpell> DirectDamageSpell::clone() const {
	return std::make_unique<DirectDamageSpell>(*this);
}
