#include "TrapSpell.h"
#include "GameField.h"

TrapSpell::TrapSpell(int damage, int radius)
	: Spell(damage, radius) {
}

bool TrapSpell::Cast(GameField& field, int targetX, int targetY) {
	if (!IsInRange(field, targetX, targetY)) {
		return false;
	}
	
	if (!field.IsCellPassable(targetX, targetY)) {
		return false;
	}
	
	field.PlaceTrap(targetX, targetY, currentDamage);
	return true;
}

std::string TrapSpell::GetName() const {
	return "Trap";
}

void TrapSpell::Enhance() {
	EnhanceBase();
}
