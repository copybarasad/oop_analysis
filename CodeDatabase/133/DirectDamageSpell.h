#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"
#include "GameField.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int radius;
    int enhancedRadius;

public:
	DirectDamageSpell(int dmg, int r);
	bool cast(GameField& field, int targetX, int targetY) override;
	void enhance() override;
	std::string getName() const override;
	SpellId getId() const override;
};

#endif