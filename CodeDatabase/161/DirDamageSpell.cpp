#include "DirDamageSpell.h"
#include "../../map/Field.h" // Добавляем включение Field.h
#include <algorithm>
#include <cmath>

DirDamageSpell::DirDamageSpell(int range, int damage)
	: range_(std::max(2, range)), damage_(std::max(10, damage)) {}

DirDamageSpell::DirDamageSpell() : DirDamageSpell(2, 10) {}

int DirDamageSpell::getDamage() const {
	return damage_;
}

int DirDamageSpell::getRange() const {
	return range_;
}

bool DirDamageSpell::use(const SpellContext& context) {
	if (!context.caster || !context.target || !context.field) {
		return false;
	}

	int x_c, y_c;
	context.field->GetPosEntity(context.caster, x_c, y_c);

	int x_t, y_t;
	context.field->GetPosEntity(context.target, x_t, y_t);

	int dx = x_c - x_t;
	int dy = y_c - y_t;
	int distanceSquared = dx * dx + dy * dy;

	if (distanceSquared > range_ * range_) {
		return false;
	}

	context.target->CauseDamage(damage_);


	return true;
}

void DirDamageSpell::upgrade(int coeficient) {
	range_ *= coeficient;
}

SpellType DirDamageSpell::getSpellType() const {
	return SpellType::DirDmg;
}

TokenSpell DirDamageSpell::serialise() const {
	TokenSpell token;
	token.type = "DirDmg";
	token.damage = std::to_string(damage_);
	token.range = std::to_string(range_);
	return token;
}