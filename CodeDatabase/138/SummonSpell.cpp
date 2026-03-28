#include "SummonSpell.h"
#include "GameField.h"
#include "Ally.h"

SummonSpell::SummonSpell(int allyCount, int radius)
	: Spell(0, radius), baseAllyCount(allyCount), currentAllyCount(allyCount) {
}

bool SummonSpell::Cast(GameField& field, int targetX, int targetY) {
	if (!IsInRange(field, targetX, targetY)) {
		return false;
	}
	
	int playerX = field.GetPlayerX();
	int playerY = field.GetPlayerY();
	int spawned = 0;
	for (int dy = -1; dy <= 1 && spawned < currentAllyCount; ++dy) {
		for (int dx = -1; dx <= 1 && spawned < currentAllyCount; ++dx) {
			if (dx == 0 && dy == 0) continue;
			
			int spawnX = targetX + dx;
			int spawnY = targetY + dy;
			
			if (!field.IsValidPosition(spawnX, spawnY)) continue;
			if (!field.IsCellPassable(spawnX, spawnY)) continue;
			if (field.IsEnemyAt(spawnX, spawnY)) continue;
			if (field.IsAllyAt(spawnX, spawnY)) continue;
			if (field.IsBuildingAt(spawnX, spawnY)) continue;
			if (field.IsTowerAt(spawnX, spawnY)) continue;
			if (spawnX == playerX && spawnY == playerY) continue;
			
			int allyHealth = 66;
			int allyDamage = 13;
			Ally newAlly(spawnX, spawnY, allyHealth, allyDamage);
			field.AddAlly(newAlly);
			spawned++;
		}
	}
	
	return spawned > 0;
}

std::string SummonSpell::GetName() const {
	return "Summon";
}

void SummonSpell::Enhance() {
	currentAllyCount = 2;
}
