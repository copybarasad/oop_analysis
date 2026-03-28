#include "areadamagespell.h"
#include "player.h"
#include "spell_context.h"
#include "field.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize) 
	: damage(damage), range(range), areaSize(areaSize), temporaryEnhancement(0), permanentEnhancement(0) {}

bool AreaDamageSpell::cast(SpellContext& context) {
	Player& player = context.getPlayer();
	
	if (!player.consumeMana(getManaCost())) {
		std::cout << "Not enough mana for " << getName() << " Spell!\n";
			return false;
	}
	
	int currentAreaSize = areaSize + temporaryEnhancement + permanentEnhancement;
	
	AreaSelection selection = context.selectArea(currentAreaSize);

	if (selection.cancelled) {
		player.restoreMana(getManaCost());
		return false;
	}
	
	bool hitTarget = false;
	
	for (int deltaX = 0; deltaX < currentAreaSize; ++deltaX) {
		for (int deltaY = 0; deltaY < currentAreaSize; ++deltaY) {
			int checkX = selection.targetX + deltaX;
			int checkY = selection.targetY + deltaY;
			
			if (!context.getField().isValidPosition(checkX, checkY)) continue;
			
			if (context.damageEntitiesAtPosition(checkX, checkY, damage)) {
				// Сущность только информирует о событии, не вызывая напрямую логирование
				context.notifySpellAreaHit(getName(), checkX, checkY, damage);
				hitTarget = true;
			}
		}
	}
	
	if (!hitTarget) {
		// Сущность только информирует о событии, не вызывая напрямую логирование
		context.notifySpellNoTargets(getName());
	}
	
	temporaryEnhancement = 0;
	return true;
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
	return std::make_unique<AreaDamageSpell>(*this);
}

void AreaDamageSpell::enhance(int enhancementLevel) {
	temporaryEnhancement += enhancementLevel;
}

void AreaDamageSpell::applyPermanentEnhancement(int enhancementLevel) {
	permanentEnhancement += enhancementLevel;
}

int AreaDamageSpell::getEnhancementLevel() const {
	return temporaryEnhancement + permanentEnhancement;
}
