#include <iostream>
#include <format>
#include <stdexcept>
#include "heroHand.h"
#include "entity.h"

#ifndef HERO_H
#define HERO_H

class Hero : public Entity
{
public:
	enum Ranges 
	{
		MELEE = 1,
		RANGED = 3,
	};
private:
	int shortDamage;
	int longDamage;
	Ranges currentRange;
	HeroHand hand;
public:
	Hero();
	Hero(int health, int points, int shortDamage, int longDamage, int handSize);
	Hero(const Hero& other);
	~Hero() = default;
	Hero& operator=(const Hero& other);

	int getDamage() const;
	int getRange() const;

	int getShortDamage() const;
	int getLongDamage() const;
	HeroHand& getHand();

	void spendPoints(int price);
	void gainHealth(int healthValue);
	void swapRanges();

	//SPELLS
	bool addSpell(Spell* spell);
	Spell* removeSpell(int id);
	Spell* getSpell(int id);
	void showHand() const;
	const HeroHand& getHand() const;
	int getHandSize() const;

	//SETTERS
	void setShortDamage(int value);
	void setLongDamage(int value);
	void setHand(HeroHand value);
};


#endif  // HERO_H