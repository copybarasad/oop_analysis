#include "EnhancementSpell.h"
#include "GameField.h"

EnhancementSpell::EnhancementSpell() {
}

bool EnhancementSpell::Cast(GameField& field, int targetX, int targetY) {
	return true;
}

std::string EnhancementSpell::GetName() const {
	return "Enhancement";
}

void EnhancementSpell::Enhance() {
}

bool EnhancementSpell::IsEnhancement() const {
	return true;
}
