#include "spellTrap.h"

SpellTrap::SpellTrap() : damage(0), name(""), price(0), range(0){}
SpellTrap::SpellTrap(int damage, int price, std::string name) : damage(damage), name(name), price(price), range(0){}

bool SpellTrap::useSpell(Game& game, int user_id)
{
	std::pair<int, int> heroPos = game.getHeroPos();
	if(!game.getField().isCellTrapped(heroPos.first, heroPos.second))
	{
		game.getField().setTrappedCell(heroPos.first, heroPos.second, damage);
		return true;
	} else
	{
		return false;
	}
}

int SpellTrap::getPrice() const
{
	return price;
}

void SpellTrap::showSpellStats()
{
	std::cout << std::format("{}. Price: {}. Damage: {}. Type: Trap", name, price, damage) << std::endl;
}

Spell* SpellTrap::clone()
{
	return new SpellTrap(*this);
}

void SpellTrap::serialize(std::ostream& os)
{
	os << "SpellTrap" << " " << damage << " " << range << " " << price << " " << name.length() << " " << name << " ";
}
void SpellTrap::deserialize(std::istream& in)
{
	in >> damage >> range >> price;
	size_t nameLength;
    in >> nameLength;
    if (!in.good()) throw std::runtime_error("Failed to read Area Damage Spell");
    if(nameLength < 0 or damage < 0 or range < 0 or price < 0)
	{
		throw std::invalid_argument("Wrong arguments for Spell Trap!");
	}
    name.resize(nameLength+1);
    in.read(&name[0], nameLength+1);
    if (in.fail()) {
        throw std::runtime_error("I/O error occurred!");
    }
}
