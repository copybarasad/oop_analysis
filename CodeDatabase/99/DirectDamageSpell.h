#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

// Direct damage spell - targets single enemy
// Only casts if target is an enemy
class DirectDamageSpell : public Spell {
private:
	int damage;

public:
	DirectDamageSpell(int range, int damage, const std::string& name);

	// Casts spell on single target
	// Returns true only if target is enemy and spell was cast
	// Outputs damage information to console
	bool cast(Field& field, const Position& casterPos, const Position& targetPos) override;
};

#endif // DIRECTDAMAGESPELL_H
