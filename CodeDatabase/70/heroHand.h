#ifndef HEROHAND_H
#define HEROHAND_H

#include <vector>
#include <iostream>
#include <format>
#include <stdexcept>
#include "spell.h"
#include "spellsFactory.h"
#include <cstdlib>
#include <ctime>

class HeroHand
{
private:
	int handSize;
	std::vector<Spell*> hand;
public:
	HeroHand();
	HeroHand(int handSize);
	HeroHand(const HeroHand& other);
	HeroHand& operator=(const HeroHand& other);
	~HeroHand();
	bool addSpell(Spell* spell);
	Spell* removeSpell(int id);
	Spell* getSpell(int id);
	void showHand() const;
	bool isFull();
	int getSize() const;

	int getHandSize() const;
	std::vector<Spell*> getHand() const;

	void setHandSize(int value);
	void setHand(std::vector<Spell*> vector);
	void deleteRandomHalf();
};

#endif