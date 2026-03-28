#include "SummonSpell.h"
#include "GameField.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

SummonSpell::SummonSpell(int health, int damage)
	: allyHealth(health), allyDamage(damage), enhancedCount(0) {}

bool SummonSpell::cast(GameField& field, int targetX, int targetY) {
    (void)targetX;
    (void)targetY;
    
	Position playerPos = field.getPlayerPosition();
	std::vector<Position> adjacentPositions = {
		{playerPos.x - 1, playerPos.y}, {playerPos.x + 1, playerPos.y},
		{playerPos.x, playerPos.y - 1}, {playerPos.x, playerPos.y + 1}
	};

	int summoned = 0;
	const int toSummon = 1 + enhancedCount;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(adjacentPositions.begin(), adjacentPositions.end(), gen);

	for (const auto& pos : adjacentPositions) {
		if (summoned >= toSummon) {
			break;
		}

		if (field.isValidPosition(pos.x, pos.y) &&
			field.getGrid()[pos.y][pos.x].isPassable() &&
			!field.getGrid()[pos.y][pos.x].hasPlayerOnCell() &&
			!field.getGrid()[pos.y][pos.x].hasEnemyOnCell() &&
			!field.getGrid()[pos.y][pos.x].hasAllyOnCell() &&
			!field.getGrid()[pos.y][pos.x].hasBuildingOnCell() &&
			!field.getGrid()[pos.y][pos.x].hasTrapOnCell()) {

			field.addAlly(pos.x, pos.y, allyHealth, allyDamage);
			summoned++;
			std::cout << "Призван союзник в позиции (" << pos.x << ", " << pos.y << ")!" << std::endl;
		}
	}

	if (summoned == 0) {
		std::cout << "Нет свободного места для призыва союзников!" << std::endl;
		return false;
	}

	std::cout << "Призвано " << summoned << " союзников!" << std::endl;
	enhancedCount = 0;
	return true;
}

void SummonSpell::enhance() {
	enhancedCount++;
}

std::string SummonSpell::getName() const {
	return "Призыв союзника";
}

SpellId SummonSpell::getId() const {
	return SpellId::SUMMON;
}