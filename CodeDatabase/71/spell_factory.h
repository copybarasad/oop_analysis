#pragma once
#include "spell.h"
#include <memory>
#include <string>

enum class SpellType {
	DirectDamage,
	AreaDamage,
	COUNT 
};

namespace SpellFactory {
	std::unique_ptr<Spell> createSpell(SpellType type);
	std::unique_ptr<Spell> createRandomSpell();

	std::string getSpellName(SpellType type);
	int getSpellCost(SpellType type);
}