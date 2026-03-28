#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "Spell.h"
#include "GameField.h"

class SummonSpell : public Spell {
private:
	int allyHealth;
	int allyDamage;
	int enhancedCount;

public:
	SummonSpell(int health = GameConstants::DEFAULT_ALLY_HEALTH,
		int damage = GameConstants::DEFAULT_ALLY_DAMAGE);
	bool cast(GameField& field, int targetX, int targetY) override;
	void enhance() override;
	std::string getName() const override;
	SpellId getId() const override;
};

#endif