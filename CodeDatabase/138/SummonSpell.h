#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "Spell.h"

class SummonSpell : public Spell {
public:
	SummonSpell(int allyCount, int radius);
	
	bool Cast(GameField& field, int targetX, int targetY) override;
	std::string GetName() const override;
	void Enhance() override;

private:
	int baseAllyCount;
	int currentAllyCount;
};

#endif
