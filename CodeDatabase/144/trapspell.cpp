#include "trapspell.h"
#include "player.h"
#include "trap.h"
#include "spell_context.h"
#include "field.h"
#include "enemymanager.h"
#include "buildingmanager.h"
#include <iostream>

TrapSpell::TrapSpell(int damage) 
	: damage(damage), temporaryEnhancement(0), permanentEnhancement(0) {}

bool TrapSpell::cast(SpellContext& context) {
	Player& player = context.getPlayer();
	
	if (!player.consumeMana(getManaCost())) {
		std::cout << "Not enough mana for " << getName() << " Spell!\n";
		return false;
	}
	
	int directionX = 0;
	int directionY = 0;

	if (!context.requestDirection("Enter direction to place trap", directionX, directionY)) {
		player.restoreMana(getManaCost());
		return false;
	}
	
	int trapX = player.getX() + directionX;
	int trapY = player.getY() + directionY;
	
	Field& field = context.getField();
	
	if (!field.isValidPosition(trapX, trapY) || 
		!field.isCellPassable(trapX, trapY)) {
		std::cout << "Cannot place trap there!\n";
		player.restoreMana(getManaCost());
		return false;
	}
	
	if (context.getEnemyManager().isCellOccupiedByEnemy(trapX, trapY) ||
		context.getBuildingManager().isCellOccupiedByBuilding(trapX, trapY)) {
		std::cout << "Cannot place trap - cell occupied!\n";
		player.restoreMana(getManaCost());
		return false;
	}
	
	int totalEnhancement = (temporaryEnhancement + permanentEnhancement) * 5;
	context.addTrap(Trap(trapX, trapY, damage + totalEnhancement));
	// Сущность только информирует о событии, не вызывая напрямую логирование
	context.notifyTrapPlaced(getName(), trapX, trapY);
	
	temporaryEnhancement = 0;
	return true;
}

std::unique_ptr<Spell> TrapSpell::clone() const {
	return std::make_unique<TrapSpell>(*this);
}

void TrapSpell::enhance(int enhancementLevel) {
	temporaryEnhancement += enhancementLevel;
}

void TrapSpell::applyPermanentEnhancement(int enhancementLevel) {
	permanentEnhancement += enhancementLevel;
}

int TrapSpell::getEnhancementLevel() const {
	return temporaryEnhancement + permanentEnhancement;
}