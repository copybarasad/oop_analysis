#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"

class TrapSpell : public Spell {
public:
	TrapSpell(int damage, int radius);
	
	bool Cast(GameField& field, int targetX, int targetY) override;
	std::string GetName() const override;
	void Enhance() override;
};

#endif
