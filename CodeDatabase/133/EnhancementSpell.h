#ifndef ENHANCEMENT_SPELL_H
#define ENHANCEMENT_SPELL_H

#include "Spell.h"
#include "GameField.h"

class EnhancementSpell : public Spell {
public:
	bool cast(GameField& field, int targetX, int targetY) override;
	void enhance() override;
	std::string getName() const override;
	SpellId getId() const override;
};

#endif