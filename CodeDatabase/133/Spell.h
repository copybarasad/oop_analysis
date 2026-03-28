#ifndef SPELL_H
#define SPELL_H

#include <string>

class GameField;

enum class SpellId {
	DIRECT_DAMAGE,
	AREA_DAMAGE,
	TRAP,
	SUMMON,
	ENHANCEMENT
};

class Spell {
public:
	virtual ~Spell() = default;
	virtual bool cast(GameField& field, int targetX, int targetY) = 0;
	virtual void enhance() = 0;
	virtual std::string getName() const = 0;
	virtual SpellId getId() const = 0;
};

#endif