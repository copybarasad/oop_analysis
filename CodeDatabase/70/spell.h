#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <stdexcept>
#include "enemyManager.h"
#include "Menus/inputGetter.h"

class Game;

class Spell
{
public:
	virtual ~Spell() = default;
	virtual int getPrice() const = 0;
	virtual bool useSpell(Game& game, int user_id) = 0;
	virtual void showSpellStats() = 0;
	virtual Spell* clone() = 0;

	virtual void serialize(std::ostream& os) = 0;
	virtual void deserialize(std::istream& in) = 0;
};

#endif