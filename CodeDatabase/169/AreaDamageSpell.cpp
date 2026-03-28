#include "AreaDamageSpell.hpp"
#include "SpellManager.hpp"

AreaDamageSpell::AreaDamageSpell(int damage, int range)
	: damage(damage), range(range) {}

AreaDamageSpell::AreaDamageSpell(const AreaDamageSpell& other)
	: damage(other.damage), range(other.range) {}

AreaDamageSpell::AreaDamageSpell(AreaDamageSpell&& other) noexcept
	: damage(other.damage), range(other.range) {
	other.damage = 0;
	other.range = 0;
}

AreaDamageSpell& AreaDamageSpell::operator=(const AreaDamageSpell& other) {
	if (this != &other) {
		damage = other.damage;
		range = other.range;
	}
	return *this;
}

AreaDamageSpell& AreaDamageSpell::operator=(AreaDamageSpell&& other) noexcept {
	if (this != &other) {
		damage = other.damage;
		range = other.range;
		other.damage = 0;
		other.range = 0;
	}
	return *this;
}

bool AreaDamageSpell::cast(SpellManager& spellManager, const Coords& casterPos, const Coords& target) {
	if (spellManager.manhattan(casterPos, target) > range) {
		return false;
	}
	
	spellManager.dealAreaDamage(target, 2, 2, damage);
	return true;
}

int AreaDamageSpell::getRange() const noexcept {
	return range;
}

int AreaDamageSpell::getDamage() const noexcept {
	return damage;
}

std::string AreaDamageSpell::getName() const {
	return "Урон по площади";
}

std::string AreaDamageSpell::getDescription() const {
	return "Наносит " + std::to_string(damage) + " урона по области 2x2 в радиусе " + std::to_string(range);
}

std::unique_ptr<ISpell> AreaDamageSpell::clone() const {
	return std::make_unique<AreaDamageSpell>(*this);
}
