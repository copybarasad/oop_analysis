#include "../h/AreaDamageSpell.h"
#include "../h/Unit.h"
#include <iostream>

using namespace std;

AreaDamageSpell::AreaDamageSpell(int range, int damage, const string& name)
	: Spell( range, name), damage(damage) {
}

// Casts area damage spell on 2x2 area
// Damages all enemies in the area
bool AreaDamageSpell::cast(Field& field, const Position& casterPos, const Position& targetPos) {
	// Check if target is in range
	if (!isInRange(casterPos, targetPos)) {
		cout << "Target out of range!" << endl;
		return false;
	}

	// Check if target position is valid
	if (!field.isValidPosition(targetPos)) {
		return false;
	}

	int enemiesHit = 0;
	int totalDamage = 0;

	// Damage all units in 2x2 area starting from targetPos
	for (int dy = 0; dy < 2; dy++) {
		for (int dx = 0; dx < 2; dx++) {
			Position areaPos = {targetPos.x + dx, targetPos.y + dy};

			// Check if position is valid
			if (!field.isValidPosition(areaPos)) {
				continue;
			}

			// Get unit at position
			Unit* target = field.getUnitAt(areaPos);

			// If there's an enemy, damage it
			if (target != nullptr && field.getCellState(areaPos) == CellState::Enemy) {
				int hpBefore = target->getHP();
				target->takeDamage(damage);
				int hpAfter = target->getHP();
				int actualDamage = hpBefore - hpAfter;
				
				totalDamage += actualDamage;
				enemiesHit++;
				
				cout << "Hit enemy at (" << areaPos.x << "," << areaPos.y << ") for " 
				     << actualDamage << " damage";
				if (hpAfter > 0) {
					cout << " (HP: " << hpAfter << ")" << endl;
				} else {
					cout << " (Defeated!)" << endl;
				}
			}
		}
	}

	if (enemiesHit == 0) {
		cout << "Area spell cast, but no enemies hit!" << endl;
	} else {
		cout << "Total: " << enemiesHit << " enemies hit for " << totalDamage << " damage!" << endl;
	}

	// Spell always casts if in range (even if no enemies hit)
	return true;
}
