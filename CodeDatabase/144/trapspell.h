#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell.h"
#include "spell_context.h"
#include <string>

class TrapSpell : public Spell {
private:
	int damage;
	int temporaryEnhancement;
	int permanentEnhancement;

public:
	TrapSpell(int damage = 30);
	bool cast(SpellContext& context) override;
	std::unique_ptr<Spell> clone() const override;
	void enhance(int enhancementLevel) override;
	void applyPermanentEnhancement(int enhancementLevel) override;
	int getEnhancementLevel() const override;
	
	std::string getName() const override { return "Trap"; }
	std::string getDescription() const override { 
		return "Places trap dealing " + std::to_string(damage) + " damage"; 
	}
	int getManaCost() const override { return 10; }
	SpellType getSpellType() const override { return SpellType::TRAP; }
};

#endif