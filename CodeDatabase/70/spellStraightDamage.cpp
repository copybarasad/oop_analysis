#include "spellStraightDamage.h"

SpellStraightDamage::SpellStraightDamage() : damage(0), range(0), price(0), name(""){}
SpellStraightDamage::SpellStraightDamage(int damage, int range, int price, std::string name) : damage(damage), range(range), price(price), name(name){}

bool SpellStraightDamage::useSpell(Game& game, int user_id)
{
	const int HERO_ID = 0;
	const int TOWER_ID = 1;
	if(user_id == HERO_ID)
	{
		std::pair<int, int> target = InputGetter::getInstance().getCell(0, 0, game.getField().getWidth(), game.getField().getHeight());
		std::pair<int, int> heroPos = game.getHeroPos();
		if(game.isCellInRange(heroPos.first, heroPos.second, target.first, target.second, range))
		{
			if(game.getEnemyManager().isEnemyInPos(target.first, target.second))
			{
				std::cout << std::format("{} was used!\n", name);
				game.attackEnemy(target.first, target.second, damage);
				return true;
			}else if(game.getEnemyBuildingManager().isBuildingInPos(target.first, target.second))
			{
				std::cout << std::format("{} was used!\n", name);
				game.attackBuilding(target.first, target.second, damage);
				return true;
			} else
			{
				return false;
			}
		} else
		{
			return false;
		}
	} else if (user_id == TOWER_ID)
	{
		std::cout << std::format("{} was used by tower!\n", name);
		game.getHero().receiveDamage(damage);
		return true;
	} else 
	{
		return false;
	}
}
int SpellStraightDamage::getPrice() const
{
	return price;
}
void SpellStraightDamage::showSpellStats()
{
	std::cout << std::format("{}. Price: {}. Damage: {}. Range: {}. Type: Straight", name, price, damage, range) << std::endl;
}

Spell* SpellStraightDamage::clone()
{
	return new SpellStraightDamage(*this);
}

void SpellStraightDamage::serialize(std::ostream& os)
{
	os << "SpellStraightDamage" << " " << damage << " " << range << " " << price << " " << name.length() << " " << name << " ";
}
void SpellStraightDamage::deserialize(std::istream& in)
{	
	in >> damage >> range >> price;
	size_t nameLength;
    in >> nameLength;
    if (!in.good()) throw std::runtime_error("Failed to read Area Damage Spell");
    if(nameLength < 0 or damage < 0 or range < 0 or price < 0)
	{
		throw std::invalid_argument("Wrong arguments for Spell of Straight Damage!");
	}
    name.resize(nameLength+1);
    in.read(&name[0], nameLength+1);
    if (in.fail()) {
        throw std::runtime_error("I/O error occurred!");
    }
}