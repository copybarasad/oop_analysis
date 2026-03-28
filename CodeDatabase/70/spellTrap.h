#ifndef SPELLTRAP_H
#define SPELLTRAP_H

#include "spell.h"
#include "game.h"
#include <string>
#include <format>
#include <iostream>

class SpellTrap: public Spell
{
private:
	int damage;
	int range;
	int price;
	std::string name;
public:
	SpellTrap();
	SpellTrap(int damage, int price, std::string name);
	~SpellTrap() = default;
	bool useSpell(Game& game, int user_id);
	int getPrice() const;
	void showSpellStats();
	Spell* clone();

	void serialize(std::ostream& os);
	void deserialize(std::istream& in);
};

#endif
