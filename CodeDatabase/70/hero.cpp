#include "hero.h"

Hero::Hero() : Entity(100, 0), longDamage(5), shortDamage(10), currentRange(MELEE), hand(5){}
Hero::Hero(int health, int points, int shortDamage, int longDamage, int handSize) : Entity(health, points),
	longDamage(longDamage), shortDamage(shortDamage), currentRange(MELEE){
	if (health<=0)
	{
		throw std::invalid_argument("The health and must be positive");
	}

	if (shortDamage<0 or longDamage<0)
	{
		throw std::invalid_argument("The damage and must be non-negative");
	}

	if (handSize <= 0)
	{
		throw std::invalid_argument("The handsize and must be positive");
	}
	hand = HeroHand(handSize);
}
HeroHand& Hero::getHand()
{
	return hand;
}

int Hero::getDamage() const
{
	if (currentRange == MELEE)
	{
		return shortDamage;
	} else 
	{
		return longDamage;
	}
}

int Hero::getShortDamage() const
{
	return shortDamage;
}

int Hero::getLongDamage() const
{
	return longDamage;
}

int Hero::getRange() const
{
	return currentRange;
}

void Hero::spendPoints(int price)
{
	points-=price;
}

void Hero::gainHealth(int healthValue)
{
	health += healthValue;
}

void Hero::swapRanges()
{
	if (currentRange == MELEE)
	{
		currentRange = RANGED;
	} else
	{
		currentRange = MELEE;
	}
}

Hero::Hero(const Hero& other) : Entity(other), longDamage(other.longDamage), 
	shortDamage(other.shortDamage), currentRange(other.currentRange)
{
	hand = other.hand;
}

Hero& Hero::operator=(const Hero& other)
{
    if (this != &other) {
    	Entity::operator=(other);
        longDamage = other.longDamage;
        shortDamage = other.shortDamage;
        currentRange = other.currentRange;
        hand = other.hand;
    }
    return *this;
}


// SPELLS

bool Hero::addSpell(Spell* spell)
{
	return hand.addSpell(spell);
}

Spell* Hero::removeSpell(int id)
{
	return hand.removeSpell(id); 
}

Spell* Hero::getSpell(int id)
{
	return hand.getSpell(id); 
}

void Hero::showHand() const
{
	hand.showHand();
}

const HeroHand& Hero::getHand() const
{
	return hand;
}

int Hero::getHandSize() const
{
	return hand.getSize();
}
//SETTERS
void Hero::setShortDamage(int value){shortDamage = value;}
void Hero::setLongDamage(int value){longDamage = value;}
void Hero::setHand(HeroHand value){hand = value;}

std::istream& operator >> (std::istream& in, HeroHand& heroHand);
std::ostream& operator << (std::ostream &os, const HeroHand &heroHand);

std::ostream& operator << (std::ostream &os, const Hero &hero)
{
	os << hero.getHealth() << " " << hero.getShortDamage() << " " << hero.getLongDamage() << " ";
	os << hero.getPoints() << " " << hero.getHand();
	return os;
}

std::istream& operator >> (std::istream& in, Hero& hero)
{
	int health;
	int points;
	int shortDamage;
	int longDamage;
	HeroHand hand;
	in >> health >> shortDamage >> longDamage >> points >> hand;
	if (!in.good()) throw std::runtime_error("Failed to read hero sizes");
	if(health <= 0 or shortDamage <= 0 or longDamage <= 0)
	{
		throw std::invalid_argument("Wrong arguments for Hero!");
	}
	hero.setHealth(health);
	hero.setShortDamage(shortDamage);
	hero.setLongDamage(longDamage);
	hero.setPoints(points);
	hero.setHand(hand);
	return in;
}
