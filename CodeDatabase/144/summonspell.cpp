#include "summonspell.h"
#include "player.h"
#include "ally.h"
#include "field.h"
#include "enemymanager.h"
#include "buildingmanager.h"
#include <iostream>
#include <vector>

SummonSpell::SummonSpell(int count) 
	: summonCount(count), temporaryEnhancement(0), permanentEnhancement(0) {}

bool SummonSpell::cast(SpellContext& context) {
	Player& player = context.getPlayer();
	
	if (!player.consumeMana(getManaCost())) {
		std::cout << "Not enough mana for " << getName() << " Spell!\n";
		return false;
	}
	
	int currentCount = summonCount + temporaryEnhancement + permanentEnhancement;
	int summonedAllies = 0;
	
	std::vector<std::pair<int, int>> possiblePositions;
	for (int dx = -1; dx <= 1; ++dx) {
		for (int dy = -1; dy <= 1; ++dy) {
			if (dx == 0 && dy == 0) continue;
			
			int allyX = player.getX() + dx;
			int allyY = player.getY() + dy;
			
			if (context.getField().isValidPosition(allyX, allyY) &&
				context.getField().isCellPassable(allyX, allyY) &&
				!context.isCellOccupiedByEnemy(allyX, allyY) &&
				!context.getBuildingManager().isCellOccupiedByBuilding(allyX, allyY) &&
				!context.isCellOccupiedByAlly(allyX, allyY)) {
				possiblePositions.emplace_back(allyX, allyY);
			}
		}
	}
	
	for (size_t i = 0; i < possiblePositions.size() && summonedAllies < currentCount; ++i) {
		int allyX = possiblePositions[i].first;
		int allyY = possiblePositions[i].second;
		context.addAlly(Ally(50, 5, allyX, allyY));
		// Сущность только информирует о событии, не вызывая напрямую логирование
		context.notifyAllySummoned(1, allyX, allyY);
		summonedAllies++;
	}
	
	temporaryEnhancement = 0;
	return summonedAllies > 0;
}

std::unique_ptr<Spell> SummonSpell::clone() const {
	return std::make_unique<SummonSpell>(*this);
}

void SummonSpell::enhance(int enhancementLevel) {
	temporaryEnhancement += enhancementLevel;
}

void SummonSpell::applyPermanentEnhancement(int enhancementLevel) {
	permanentEnhancement += enhancementLevel;
}

int SummonSpell::getEnhancementLevel() const {
	return temporaryEnhancement + permanentEnhancement;
}