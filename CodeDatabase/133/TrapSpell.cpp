#include "TrapSpell.h"
#include "GameField.h"
#include <iostream>

TrapSpell::TrapSpell(int dmg) : damage(dmg), enhancedDamage(0) {}

bool TrapSpell::cast(GameField& field, int targetX, int targetY) {
	if (!field.isValidPosition(targetX, targetY) || !field.getGrid()[targetY][targetX].isPassable()) {
		std::cout << "Невозможно разместить ловушку в этой позиции!" << std::endl;
		return false;
	}

	if (field.getGrid()[targetY][targetX].hasTrapOnCell()) {
		std::cout << "В этой позиции уже есть ловушка!" << std::endl;
		return false;
	}

	field.addTrap(targetX, targetY, damage + enhancedDamage);
	std::cout << "Ловушка установлена в позиции (" << targetX << ", " << targetY
		<< ") с уроном " << (damage + enhancedDamage) << "!" << std::endl;

	enhancedDamage = 0;
	return true;
}

void TrapSpell::enhance() {
	enhancedDamage += 5;
}

std::string TrapSpell::getName() const {
	return "Ловушка";
}

SpellId TrapSpell::getId() const {
	return SpellId::TRAP;
}