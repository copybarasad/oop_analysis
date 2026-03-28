#ifndef SPELLAREADAMAGE_H
#define SPELLAREADAMAGE_H

#include "spell.h"
#include "game.h"
#include <string>
#include <format>
#include <iostream>


class SpellAreaDamage: public Spell
{
private:
	int AREA_WIDTH = 2;
	int AREA_HEIGHT = 2;
	int damage;
	int range;
	int price;
	std::string name;
	bool checkCorrectSpellArea(int x_down_left, int y_down_left, int x_up_right, int y_up_right, int width, int height);
	std::pair<int, int> getSquareCenter(int x_down_left, int y_down_left, int x_up_right, int y_up_right);
public:
	SpellAreaDamage();
	SpellAreaDamage(int damage, int range, int price, std::string name);
	~SpellAreaDamage() = default;
	bool useSpell(Game& game, int user_id);
	int getDamage() const;
	int getRange() const;
	int getPrice() const;
	std::string getName() const;
	int getAreaWidth() const;
	int getAreaHeight() const;
	void showSpellStats();
	Spell* clone();

	void serialize(std::ostream& os);
	void deserialize(std::istream& in);
};

#endif