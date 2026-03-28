#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "spell.h"
#include "spell_context.h"
#include <string>

class AreaDamageSpell : public Spell {
private:
	int damage;
	int range;
	int areaSize;
	int temporaryEnhancement;
	int permanentEnhancement;

public:
	AreaDamageSpell(int damage = 150, int range = 2, int areaSize = 2);
	bool cast(SpellContext& context) override;
	std::unique_ptr<Spell> clone() const override;
	void enhance(int enhancementLevel) override;
	void applyPermanentEnhancement(int enhancementLevel) override;
	int getEnhancementLevel() const override;
	
	std::string getName() const override { return "Area Damage"; }
	std::string getDescription() const override { 
		return "Deals " + std::to_string(damage) + " damage in " + 
			   std::to_string(areaSize) + "x" + std::to_string(areaSize) + " area"; 
	}
	int getManaCost() const override { return 25; }
	SpellType getSpellType() const override { return SpellType::AREA_DAMAGE; }
};

#endif