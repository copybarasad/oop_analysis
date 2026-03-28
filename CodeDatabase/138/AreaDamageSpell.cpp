#include "AreaDamageSpell.h"
#include "GameField.h"

AreaDamageSpell::AreaDamageSpell(int damage, int radius, int areaSize)
	: Spell(damage, radius), baseAreaSize(areaSize), currentAreaSize(areaSize) {
}

bool AreaDamageSpell::Cast(GameField& field, int targetX, int targetY) {
	if (!IsInRange(field, targetX, targetY)) {
		return false;
	}
	
	int halfSize = currentAreaSize / 2;
	int startX = targetX - halfSize;
	int startY = targetY - halfSize;
	
	field.AttackArea(startX, startY, currentAreaSize, currentDamage);
	
	return true;
}

std::string AreaDamageSpell::GetName() const {
	return "Area Damage";
}

void AreaDamageSpell::Enhance() {
	EnhanceBase();
	currentAreaSize = static_cast<int>(baseAreaSize * 1.1);
	if (currentAreaSize < 2) {
		currentAreaSize = 2;
	}
}
