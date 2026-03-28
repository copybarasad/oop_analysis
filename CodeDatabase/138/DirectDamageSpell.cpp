#include "DirectDamageSpell.h"
#include "GameField.h"

DirectDamageSpell::DirectDamageSpell(int damage, int radius)
	: Spell(damage, radius) {
}

bool DirectDamageSpell::Cast(GameField& field, int targetX, int targetY) {
	if (!IsInRange(field, targetX, targetY)) {
		return false;
	}
	
	bool targetFound = false;
	
	if (field.IsEnemyAt(targetX, targetY)) {
		field.AttackEnemyAt(targetX, targetY, currentDamage);
		targetFound = true;
	}
	
	if (field.IsBuildingAt(targetX, targetY)) {
		field.AttackBuildingAt(targetX, targetY, currentDamage);
		targetFound = true;
	}
	
	return targetFound;
}

std::string DirectDamageSpell::GetName() const {
	return "Direct Damage";
}

void DirectDamageSpell::Enhance() {
	EnhanceBase();
}
