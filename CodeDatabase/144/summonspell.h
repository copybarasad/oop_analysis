#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "spell.h"
#include "spell_context.h"
#include <string>

class SummonSpell : public Spell {
private:
	int summonCount;
	int temporaryEnhancement;
	int permanentEnhancement;

public:
	SummonSpell(int count = 1);
	bool cast(SpellContext& context) override;
	std::unique_ptr<Spell> clone() const override;
	void enhance(int enhancementLevel) override;
	void applyPermanentEnhancement(int enhancementLevel) override;
	int getEnhancementLevel() const override;
	
	std::string getName() const override { return "Summon Ally"; }
	std::string getDescription() const override { 
		return "Summons " + std::to_string(summonCount) + " ally to fight for you"; 
	}
	int getManaCost() const override { return 30; }
	SpellType getSpellType() const override { return SpellType::SUMMON; }
};

#endif