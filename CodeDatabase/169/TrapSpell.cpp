#include "TrapSpell.hpp"
#include "SpellManager.hpp"

TrapSpell::TrapSpell(int damage, int range)
	: damage(damage), range(range) {}

TrapSpell::TrapSpell(const TrapSpell& other)
	: damage(other.damage), range(other.range) {}

TrapSpell::TrapSpell(TrapSpell&& other) noexcept
	: damage(other.damage), range(other.range) {
	other.damage = 0;
	other.range = 0;
}

TrapSpell& TrapSpell::operator=(const TrapSpell& other) {
	if (this != &other) {
		damage = other.damage;
		range = other.range;
	}
	return *this;
}

TrapSpell& TrapSpell::operator=(TrapSpell&& other) noexcept {
	if (this != &other) {
		damage = other.damage;
		range = other.range;
		other.damage = 0;
		other.range = 0;
	}
	return *this;
}

bool TrapSpell::cast(SpellManager& spellManager, const Coords& casterPos, const Coords& target) {
	if (spellManager.manhattan(casterPos, target) > range) {
		return false;
	}
	
	return spellManager.placeTrap(target, damage);
}

int TrapSpell::getRange() const noexcept {
	return range;
}

int TrapSpell::getDamage() const noexcept {
	return damage;
}

std::string TrapSpell::getName() const {
	return "Ловушка";
}

std::string TrapSpell::getDescription() const {
	return "Размещает ловушку (урон " + std::to_string(damage) + ") в радиусе " + std::to_string(range);
}

std::unique_ptr<ISpell> TrapSpell::clone() const {
	return std::make_unique<TrapSpell>(*this);
}
