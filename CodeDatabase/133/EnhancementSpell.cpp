#include "EnhancementSpell.h"
#include "GameField.h"
#include <iostream>

bool EnhancementSpell::cast(GameField& field, int targetX, int targetY) {
	(void)targetX;
	(void)targetY;

	field.accumulateEnhancement();
	std::cout << "Заклинание улучшения применено! Следующее заклинание будет усилено." << std::endl;
	return true;
}

void EnhancementSpell::enhance() {
}

std::string EnhancementSpell::getName() const {
	return "Улучшение";
}

SpellId EnhancementSpell::getId() const {
	return SpellId::ENHANCEMENT;
}