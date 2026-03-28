#include "spell_state.h"
#include "directdamagespell.h"
#include "areadamagespell.h"
#include "trapspell.h"
#include "summonspell.h"
#include "enhancementspell.h"
#include <stdexcept>

SpellState SpellSerializer::serialize(const Spell& spell) const {
	SpellState state{spell.getSpellType(), spell.getEnhancementLevel()};
	return state;
}

std::unique_ptr<Spell> SpellSerializer::deserialize(const SpellState& state) const {
	std::unique_ptr<Spell> spell;
	bool applyEnhancementLevel = true;
	switch (state.type) {
		case SpellType::DIRECT_DAMAGE:
			spell = std::make_unique<DirectDamageSpell>();
			break;
		case SpellType::AREA_DAMAGE:
			spell = std::make_unique<AreaDamageSpell>();
			break;
		case SpellType::TRAP:
			spell = std::make_unique<TrapSpell>();
			break;
		case SpellType::SUMMON:
			spell = std::make_unique<SummonSpell>();
			break;
		case SpellType::ENHANCEMENT: {
			int storedLevel = state.enhancementLevel > 0 ? state.enhancementLevel : 1;
			spell = std::make_unique<EnhancementSpell>(storedLevel);
			applyEnhancementLevel = false; // уровень уже учтен в конструкторе
			break;
		}
		default:
			throw std::runtime_error("Unknown spell type during reconstruction");
	}
	if (applyEnhancementLevel && state.enhancementLevel > 0) {
		spell->enhance(state.enhancementLevel);
	}
	return spell;
}


