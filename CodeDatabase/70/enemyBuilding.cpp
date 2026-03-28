#include "enemyBuilding.h"
EnemyBuilding::EnemyBuilding() : range(0), reward(0), health(0), spell(nullptr){}
EnemyBuilding::EnemyBuilding(int range, int reward, int health) : range(range), reward(reward), health(health), spell(nullptr){}
void EnemyBuilding::addSpell(Spell* spell)
{
	if(this->spell)
	{
		delete this->spell;
	}
	if(spell)
	{
		this->spell = spell->clone();
	} else 
	{
		this->spell = nullptr;
	}
}

EnemyBuilding::~EnemyBuilding()
{
    delete spell;
    spell = nullptr;
}

EnemyBuilding::EnemyBuilding(const EnemyBuilding& other) : range(other.range), reward(other.reward), health(other.health)
{
	if (other.spell != nullptr) 
	{
        spell = other.spell->clone();
    } else 
    {
        spell = nullptr;
    }
}
EnemyBuilding& EnemyBuilding::operator=(const EnemyBuilding& other)
{
	delete spell;
	range = other.range;
	reward = other.reward;
	health = other.health;
	if (other.spell != nullptr) 
	{
        spell = other.spell->clone();
    } else 
    {
        spell = nullptr;
    }
	return *this;
}

Spell* EnemyBuilding::useSpell()
{
	Spell* usingSpell = spell->clone();
	delete spell;
	spell = nullptr;
	return usingSpell;
}

Spell* EnemyBuilding::getSpell() const
{
	return spell;
}

int EnemyBuilding::getRange() const
{
	return range;
}
int EnemyBuilding::getReward() const
{
	return reward;
}
int EnemyBuilding::getHealth() const
{
	return health;
}
void EnemyBuilding::receiveDamage(int damageValue)
{
	health -= damageValue;
}
void EnemyBuilding::showStats() const
{
	std::cout << std::format("Health: {}. Range: {}. Reward: {}.", health, range, reward) << std::endl;
}

void EnemyBuilding::setRange(int value){range = value;}
void EnemyBuilding::setReward(int value){reward = value;}
void EnemyBuilding::setHealth(int value){health = value;}

std::ostream& operator << (std::ostream &os, const EnemyBuilding &enemyBuilding)
{
	os << enemyBuilding.getHealth() << " " << enemyBuilding.getRange() << " " << enemyBuilding.getReward() << " ";
	Spell* spell = enemyBuilding.getSpell();
	bool hasSpell = false;
	if(spell)
	{
		hasSpell = true;
		os << hasSpell << " ";
		spell->serialize(os);
		os << " ";
	} else
	{
		os << hasSpell << " ";
	}
	return os;
}

std::istream& operator >> (std::istream &in, EnemyBuilding& enemyBuilding)
{
	int health;
	int range;
	int reward;
	in >> health >> range >> reward;
	if (!in.good()) throw std::runtime_error("Failed to read enemy building");
	if(health <= 0 or range <= 0 or reward < 0)
	{
		throw std::invalid_argument("Wrong arguments for Enemy Building!");
	}
	bool hasSpell;
	in >> hasSpell;
	if(hasSpell){
		std::string spellType;
		in >> spellType;
		if (!in.good()) throw std::runtime_error("Failed to read spell type");
		Spell* spell = SpellsFactory::getInstance().createSpell(spellType, in);
		if(spell)
		{
			enemyBuilding.addSpell(spell);
		} else 
		{
			throw std::invalid_argument("Can't get Spell!");
		}
	} else 
	{
		enemyBuilding.addSpell(nullptr);
	}
	enemyBuilding.setHealth(health);
	enemyBuilding.setRange(range);
	enemyBuilding.setReward(reward);
	return in;
}