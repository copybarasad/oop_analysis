#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.h"
#include "spell_context.h"
#include <string>

class DirectDamageSpell : public Spell {
private:
	int damage;
	int range;
	int temporaryEnhancement;
	int permanentEnhancement;

public:
	DirectDamageSpell(int damage = 200, int range = 5);
	bool cast(SpellContext& context) override;
	std::unique_ptr<Spell> clone() const override;
	void enhance(int enhancementLevel) override;
	void applyPermanentEnhancement(int enhancementLevel) override;
	int getEnhancementLevel() const override;
	
	std::string getName() const override { return "Direct Damage"; }
	std::string getDescription() const override { 
		return "Deals " + std::to_string(damage) + " damage to selected target in range";
	}
	int getManaCost() const override { return 15; }
	SpellType getSpellType() const override { return SpellType::DIRECT_DAMAGE; }
private:
	bool applyDamageToTarget(SpellContext& context, const SpellTarget& target, int damageAmount);
};
#endif