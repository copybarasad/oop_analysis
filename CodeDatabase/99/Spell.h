#ifndef SPELL_H
#define SPELL_H

#include "Field.h"
#include <string>

class Unit;

// Abstract interface for all spells
// Provides unified interface for spell casting
class Spell {
protected:
	int range;
	std::string name;

public:
	Spell(int range, std::string  name);
	virtual ~Spell() = default;

	// Getters
	int getRange() const;
	std::string getName() const;

	// Pure virtual method - must be implemented by derived classes
	// Returns true if spell was successfully cast
	virtual bool cast(Field& field, const Position& casterPos, const Position& targetPos) = 0;

	// Check if target position is within spell range
	bool isInRange(const Position& casterPos, const Position& targetPos) const;
};

#endif // SPELL_H
