#include "Spell.h"
#include "GameField.h"
#include "MathUtils.h"

bool Spell::IsInRange(GameField& field, int targetX, int targetY) const {
	if (!field.IsValidPosition(targetX, targetY)) {
		return false;
	}
	
	int playerX = field.GetPlayerX();
	int playerY = field.GetPlayerY();
	int distance = MathUtils::CalculateDistance(playerX, playerY, targetX, targetY);
	
	return distance <= currentRadius;
}

