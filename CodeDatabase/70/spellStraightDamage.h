#ifndef SPELLSTRAIGHTDAMAGE_H
#define SPELLSTRAIGHTDAMAGE_H

#include "spell.h"
#include "enemyManager.h"
#include "game.h"
#include <string>
#include <format>
#include <iostream>
#include <utility>

class SpellStraightDamage: public Spell
{
private:
	int damage;
	int range;
	int price;
	std::string name;
public:
	SpellStraightDamage();
	SpellStraightDamage(int damage, int range, int price, std::string name);
	~SpellStraightDamage() = default;
	bool useSpell(Game& game, int user_id);
	int getPrice() const;
	void showSpellStats();
	Spell* clone();

	void serialize(std::ostream& os);
	void deserialize(std::istream& in);
};

#endif
