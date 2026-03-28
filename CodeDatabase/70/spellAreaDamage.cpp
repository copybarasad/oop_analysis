#include "spellAreaDamage.h"

SpellAreaDamage::SpellAreaDamage() : damage(0), range(0), price(0), name(""), AREA_HEIGHT(2), AREA_WIDTH(2){}
SpellAreaDamage::SpellAreaDamage(int damage, int range, int price, std::string name) : damage(damage), range(range), price(price), name(name), AREA_HEIGHT(2), AREA_WIDTH(2){}

std::pair<int, int> SpellAreaDamage::getSquareCenter(int x_down_left, int y_down_left, int x_up_right, int y_up_right)
{
	std::pair<int, int> center;
	center.first = (x_up_right - x_down_left)/2 + x_down_left;
	center.second = (y_up_right - y_down_left)/2 + y_down_left;
	return center;
}

bool SpellAreaDamage::checkCorrectSpellArea(int x_down_left, int y_down_left, int x_up_right, int y_up_right, int width, int height)
{
	if(((x_up_right - x_down_left) == width - 1) and ((y_up_right - y_down_left) == height - 1))
	{
		return true;
	}
	return false;
}

bool SpellAreaDamage::useSpell(Game& game, int user_id)
{
	const int HERO_ID = 0;
	std::pair<int, int> heroPos = game.getHeroPos();
	if(user_id == HERO_ID){
		std::cout << "Choose left down corner." << std::endl;
		std::pair<int, int> leftDownCorner = InputGetter::getInstance().getCell(0, 0, game.getField().getWidth(), game.getField().getHeight());
		std::cout << "Choose right up corner." << std::endl;
		std::pair<int, int> rightUpCorner = InputGetter::getInstance().getCell(0, 0, game.getField().getWidth(), game.getField().getHeight());
		if(!checkCorrectSpellArea(leftDownCorner.first, leftDownCorner.second,
							  rightUpCorner.first, rightUpCorner.second,
							  AREA_WIDTH, AREA_HEIGHT))
		{
			std::cout << "Spell Area is incorrect!" << std::endl;
			return false;
		}

		std::pair<int, int> areaCenter = getSquareCenter(leftDownCorner.first, leftDownCorner.second,
			rightUpCorner.first, rightUpCorner.second);
		std::cout << areaCenter.first << " " << areaCenter.second << std::endl;


		if(game.isCellInRange(heroPos.first, heroPos.second, areaCenter.first, areaCenter.second, range))
		{
			std::cout << std::format("{} was used!\n", name);
	    	for(size_t i = 0; i < AREA_WIDTH; ++i)
	    	{
	    		for(size_t j = 0; j < AREA_HEIGHT; ++j)
	    		{
	    			if(game.getEnemyManager().isEnemyInPos(leftDownCorner.first+i, leftDownCorner.second+j))
					{
						game.attackEnemy(leftDownCorner.first+i, leftDownCorner.second+j, damage);
					}
	    		}
	    	}
	    	return true;
	    } else
	    {
	    	std::cout << "Spell Area is not in usage range!" << std::endl;
	    	return false;
	    }
		return true;
	} else 
	{
		return false;
	}
}

int SpellAreaDamage::getDamage() const
{
	return damage;
}

int SpellAreaDamage::getRange() const
{
	return range;
}

int SpellAreaDamage::getAreaWidth() const
{
	return AREA_WIDTH;
}

int SpellAreaDamage::getAreaHeight() const
{
	return AREA_HEIGHT;
}

int SpellAreaDamage::getPrice() const
{
	return price;
}

std::string SpellAreaDamage::getName() const
{
	return name;
}

void SpellAreaDamage::showSpellStats()
{
	std::cout << std::format("{}. Price: {}. Damage: {}. Range: {}. Type: Area", name, price, damage, range) << std::endl;
}

Spell* SpellAreaDamage::clone()
{
	return new SpellAreaDamage(*this);
}

void SpellAreaDamage::serialize(std::ostream& os)
{
	os << "SpellAreaDamage" << " " << damage << " " << range << " " << price << " " << name.length() << " " << name << " ";
	os << AREA_WIDTH << " " << AREA_HEIGHT << " ";
}
void SpellAreaDamage::deserialize(std::istream& in)
{
	in >> damage >> range >> price;
	size_t nameLength;
    in >> nameLength;
    if (!in.good()) throw std::runtime_error("Failed to read Area Damage Spell");
    if(nameLength < 0 or damage < 0 or range < 0 or price < 0)
	{
		throw std::invalid_argument("Wrong arguments for Spell of Area Damage!");
	}
    name.resize(nameLength+1);
    in.read(&name[0], nameLength+1);
    if (in.fail()) {
        throw std::runtime_error("I/O error occurred!");
    }
    in >> AREA_WIDTH >> AREA_HEIGHT;
    if (!in.good()) throw std::runtime_error("Failed to read Area Damage Spell");
    if(AREA_HEIGHT < 0 or AREA_WIDTH < 0)
	{
		throw std::invalid_argument("Wrong arguments for Spell of Area Damage!");
	}
}


