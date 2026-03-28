#pragma once
#include "spell.h"
#include "spell_factory.h"

class AreaDamageSpell : public Spell {
public:
	AreaDamageSpell(int damage, float radius);
	SpellCastResult apply(Player* caster, PlayingField& field) override;
	std::string getName() const override;
	std::string getDescription() const override;
	SpellType getType() const override;

private:
	int damage_;
	float radius_;
};
