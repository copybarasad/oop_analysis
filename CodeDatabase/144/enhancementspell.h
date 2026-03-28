#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "spell.h"
#include "spell_context.h"
#include <string>

class EnhancementSpell : public Spell {
private:
	int enhancementLevel;

public:
	EnhancementSpell(int level = 1);
	bool cast(SpellContext& context) override;
	std::unique_ptr<Spell> clone() const override;
	void enhance(int enhancementLevel) override;
	void applyPermanentEnhancement(int enhancementLevel) override;
	int getEnhancementLevel() const override;
	bool isEnhancement() const override { return true; }
	
	std::string getName() const override { return "Enhancement"; }
	std::string getDescription() const override { 
		return "Enhances next spell by " + std::to_string(enhancementLevel) + " level(s)"; 
	}
	int getManaCost() const override { return 20; }
	SpellType getSpellType() const override { return SpellType::ENHANCEMENT; }
};

#endif