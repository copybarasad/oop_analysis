#include "enhancementspell.h"
#include "player.h"
#include <iostream>

EnhancementSpell::EnhancementSpell(int level) 
	: enhancementLevel(level) {}

bool EnhancementSpell::cast(SpellContext& context) {
	Player& player = context.getPlayer();
	
	if (!player.consumeMana(getManaCost())) {
		std::cout << "Not enough mana for " << getName() << " Spell!\n";
		return false;
	}
	
	player.applyEnhancement(enhancementLevel);
	std::cout << getName() << " spell cast! Current enhancement level: " 
			  << player.getCurrentEnhancementLevel() << "\n";
	
	return true;
}

std::unique_ptr<Spell> EnhancementSpell::clone() const {
	return std::make_unique<EnhancementSpell>(*this);
}

void EnhancementSpell::enhance(int additionalLevel) {
	enhancementLevel += additionalLevel;
}

void EnhancementSpell::applyPermanentEnhancement(int enhancementLevel) {
	// EnhancementSpell сам является усилением, не может быть усилен
}

int EnhancementSpell::getEnhancementLevel() const {
	return enhancementLevel;
}