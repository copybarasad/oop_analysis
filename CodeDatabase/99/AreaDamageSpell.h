#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

// Area damage spell - damages 2x2 area
// Always casts if in range, even if no enemies present
class AreaDamageSpell : public Spell {
private:
	int damage;

public:
	AreaDamageSpell(int range, int damage, const std::string& name);

	// Casts spell on 2x2 area starting from targetPos
	// Damages all enemies in the area
	// Returns true if spell was cast (always true if in range)
	// Outputs damage information to console
	bool cast(Field& field, const Position& casterPos, const Position& targetPos) override;
};

#endif // AREADAMAGESPELL_H
