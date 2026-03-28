#ifndef ENHANCEMENTSPELL_H
#define ENHANCEMENTSPELL_H

#include "Spell.h"

class EnhancementSpell : public Spell {
public:
	EnhancementSpell();
	
	bool Cast(GameField& field, int targetX, int targetY) override;
	std::string GetName() const override;
	void Enhance() override;
	bool IsEnhancement() const override;
};

#endif
