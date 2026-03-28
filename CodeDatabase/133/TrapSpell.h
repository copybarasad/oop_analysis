#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"
#include "GameField.h"

class TrapSpell : public Spell {
private:
	int damage;
	int enhancedDamage;

public:
	TrapSpell(int dmg = GameConstants::DEFAULT_TRAP_DAMAGE);
	bool cast(GameField& field, int targetX, int targetY) override;
	void enhance() override;
	std::string getName() const override;
	SpellId getId() const override;
};

#endif