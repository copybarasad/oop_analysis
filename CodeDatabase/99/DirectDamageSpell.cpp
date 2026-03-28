#include "../h/DirectDamageSpell.h"
#include "../h/Unit.h"
#include <iostream>

using namespace std;

DirectDamageSpell::DirectDamageSpell(int range, int damage, const string& name)
	: Spell(range, name), damage(damage) {
}

// Casts direct damage spell on target
// Only works if target is an enemy
bool DirectDamageSpell::cast(Field& field, const Position& casterPos, const Position& targetPos) {
	// Check if target is in range
	if (!isInRange(casterPos, targetPos)) {
		cout << "Target out of range!" << endl;
		return false;
	}

	// Check if target position is valid
	if (!field.isValidPosition(targetPos)) {
		return false;
	}

	// Get unit at target position
	Unit* target = field.getUnitAt(targetPos);

	// Check if there is an enemy at target
	if (target == nullptr) {
		cout << "No enemy at target position!" << endl;
		return false;
	}

	// Check if target is enemy (not player)
	if (field.getCellState(targetPos) != CellState::Enemy) {
		cout << "Cannot target player!" << endl;
		return false;
	}

	// Store HP before damage
	int hpBefore = target->getHP();
	
	// Apply damage to enemy
	target->takeDamage(damage);
	
	int hpAfter = target->getHP();
	int actualDamage = hpBefore - hpAfter;
	
	cout << "Hit enemy for " << actualDamage << " damage! ";
	if (hpAfter > 0) {
		cout << "(Enemy HP: " << hpAfter << ")" << endl;
	} else {
		cout << "(Enemy defeated!)" << endl;
	}

	return true;
}
