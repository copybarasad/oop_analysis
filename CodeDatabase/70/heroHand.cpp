#include "heroHand.h"

HeroHand::HeroHand(){};
HeroHand::HeroHand(int handSize): handSize(handSize){}

HeroHand::~HeroHand()
{
	for (size_t i = 0; i < hand.size(); ++i) {
        delete hand[i];
    }
    hand.clear();
}

HeroHand::HeroHand(const HeroHand& other) : handSize(other.handSize)
{
	for (size_t i = 0; i < hand.size(); ++i) {
        delete hand[i];
    }
    hand.clear();
	for (size_t i = 0; i < (other.hand).size(); ++i) {
       	hand.push_back(other.hand[i]->clone());
    }
}

HeroHand& HeroHand::operator=(const HeroHand& other)
{
	handSize = other.handSize;
	for (size_t i = 0; i < hand.size(); ++i) {
        delete hand[i];
    }
    hand.clear();
	for (size_t i = 0; i < (other.hand).size(); ++i) {
       	hand.push_back(other.hand[i]->clone());
    }
    return *this;
}

bool HeroHand::addSpell(Spell* spell)
{
	if(isFull())
	{
		std::cout << "Your hand is full!" << std::endl;
		return false;
	} else 
	{
		hand.push_back(spell);
		return true;
	}
}

Spell* HeroHand::removeSpell(int id)
{
	if(id < 0 or id >= hand.size())
	{
		throw std::invalid_argument("Invaild id of item to remove from hand.");
	}
	Spell* spell = hand[id]->clone();
	delete hand[id];
	hand.erase(hand.begin()+id);
	return spell;
}

Spell* HeroHand::getSpell(int id)
{
	if(id < 0 or id >= hand.size())
	{
		throw std::invalid_argument("Invaild id of item to get from hand.");
	}
	Spell* spell = hand[id]->clone();
	return spell;
}

void HeroHand::showHand() const
{
	if (hand.size() == 0)
	{
		std::cout << "Hand is empty!" << std::endl;;
	}
	for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << std::format("{}: ", i);
        hand[i]->showSpellStats();
    }
}

bool HeroHand::isFull()
{
	if(hand.size() == handSize)
	{
		return true;
	} else
	{
		return false;
	}
}

int HeroHand::getSize() const
{
	return hand.size();
}

int HeroHand::getHandSize() const {return handSize;}
std::vector<Spell*> HeroHand::getHand() const {return hand;}
void HeroHand::setHandSize(int value){handSize = value;}
void HeroHand::setHand(std::vector<Spell*> vector){hand = vector;}

std::ostream& operator << (std::ostream &os, const HeroHand &heroHand)
{
	os << heroHand.getHandSize() << " " << heroHand.getSize() << " ";
	for(Spell* elem : heroHand.getHand())
	{
		elem->serialize(os);
		os << " ";
	}
	return os;
}

std::istream& operator >> (std::istream& in, HeroHand& heroHand)
{
	int handSize;
	int size;
	std::vector<Spell*> hand;
	in >> handSize >> size;
	if (!in.good()) throw std::runtime_error("Failed to read hero hand parameters");
	if(handSize < 0 or size < 0)
	{
		throw std::invalid_argument("Wrong arguments for Hero Hand!");
	}
	std::string spellType;
	for(size_t i = 0; i < size; ++i)
	{
		in >> spellType;
		if (!in.good()) throw std::runtime_error("Failed to read spell type");
		Spell* spell = SpellsFactory::getInstance().createSpell(spellType, in);
		if(spell)
		{
			hand.push_back(spell);
		} else 
		{
			throw std::invalid_argument("Can't get Spell!");
		}
	}
	heroHand.setHandSize(handSize);
	heroHand.setHand(hand);
	return in;
}

void HeroHand::deleteRandomHalf()
{
	int spellsToDelete = hand.size()/2;
	for(size_t i = 0; i < spellsToDelete; ++i)
	{
		srand(time(NULL));
    	int id = (rand() % hand.size());
    	removeSpell(id);
	}
}