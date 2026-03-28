#include "PlayerHand.h"
#include "Spells/DirectDamageSpell.h"
#include "Spells/AreaDamageSpell.h"
#include "Spells/TrapSpell.h"
#include "Spells/AllySpell.h"
#include "Spells/UpgradeSpell.h"
#include <ctime>


PlayerHand::PlayerHand(Point pos, int maxSize) : maxSize(maxSize), pos(pos) 
{
	srand((unsigned int)time(0));

	switch (rand() % 5)
	{
	case 0:
		spells.push_back(new DirectDamageSpell());
		break;
	case 1:
		spells.push_back(new AreaDamageSpell());
		break;
	case 2:
		spells.push_back(new TrapSpell());
		break;
	case 3:
		spells.push_back(new AllySpell());
		break;
	case 4:
		spells.push_back(new UpgradeSpell());
		break;
	default:
		break;
	}
};


PlayerHand::PlayerHand(const PlayerHand& other)
	: pos(other.pos), maxSize(other.maxSize), upgradeCount(other.upgradeCount), indexCurrent(other.indexCurrent) 
{
	std::vector<Spell*> newSpells;

	for (auto* spell : other.spells)
	{
		newSpells.push_back(spell->getCopy());
	}

	spells = newSpells;
}


void PlayerHand::to_json(nlohmann::json& js) const
{
	js["position"] = {
		{"x", pos.X()},
		{"y", pos.Y()}
	};
	
	js["upgradeCount"] = upgradeCount;
	js["indexCurrent"] = indexCurrent;
	
	// Все заклинания в руке
	js["spells"] = nlohmann::json::array();
	for (auto* spell : spells) {
		nlohmann::json jSpell;
		spell->to_json(jSpell);
		js["spells"].push_back(jSpell);
	}

}	


void PlayerHand::from_json(const nlohmann::json& js)
{
	if (js.contains("position") && js["position"].contains("x") && js["position"].contains("y"))
	{
		pos = Point(
			js["position"]["x"],
			js["position"]["y"]
		);
	}
	else
		throw std::runtime_error(u8"Отсутствует поле position");

	// Восстанавливаем поля
	if (js.contains("upgradeCount"))
		upgradeCount = js["upgradeCount"];
	else
		throw std::runtime_error(u8"Ошибка считывания upgradeCount");

	if (js.contains("indexCurrent"))
		indexCurrent = js["indexCurrent"];
	else
		throw std::runtime_error(u8"Ошибка считывания indexCurrent");

	// Загружаем заклинания
	if (js.contains("spells") && js["spells"].is_array())
	{
		// Полностью очищаем старые
		for (auto* s : spells)
			delete s;
		spells.clear();

		// Восстанавливаем новые
		for (const auto& jSpell : js["spells"])
		{
			SpellType type;
			if (jSpell.contains("type"))
				type = jSpell["type"];
			else
				throw std::runtime_error(u8"Ошибка считывания type");

			Spell* spell = nullptr;
			switch (type)
			{
			case SpellType::AreaDamageSpell:
				spell = new AreaDamageSpell();
				break;
			case SpellType::DirectDamageSpell:
				spell = new DirectDamageSpell();
				break;
			case SpellType::TrapSpell:
				spell = new TrapSpell();
				break;
			case SpellType::AllySpell:
				spell = new AllySpell();
				break;
			case SpellType::UpgradeSpell:
				spell = new UpgradeSpell();
				break;
			default:
				continue; // Просто пропускаем неизвестное
			}

			try {
				spell->from_json(jSpell);
			}
			catch(...){
				throw std::runtime_error(u8"Не удалось считать spell.");
			}

			spells.push_back(spell);
		}
	}
	else
		throw std::runtime_error(u8"Отсутствует поле spells, либо оно не корректно.");
}


void PlayerHand::setPosition(const Point& point)
{
	pos = point;
}


void PlayerHand::removeRandomSpells(int count)
{
	srand((unsigned int)time(0));
	int length = spells.size();

	count = count <= spells.size() ? count : length;

	for (int i = 0; i < count; ++i)
		spells.erase(spells.begin() + rand() % (length--));
}


int PlayerHand::getSpellsCount() const
{
	return spells.size();
}


std::string PlayerHand::getSpellNameInTheHand() const
{
	if (indexCurrent < spells.size())
		return spells[indexCurrent]->getName() + "( lvl " + std::to_string(spells[indexCurrent]->getLevel()) + ")";
	return "None";
}


std::string PlayerHand::getSpellInfo() const
{
	if (indexCurrent < spells.size())
		return spells[indexCurrent]->getInfo();
	return "";
}


const std::vector<Spell*> PlayerHand::getSpells() const
{
	return spells;
}


int PlayerHand::getMaxArmSize() const
{
	return maxSize;
}


void PlayerHand::takeSpell(int index)
{
	if (index < spells.size())
	{
		indexCurrent = index;
		for (int i = 0; i < upgradeCount; ++i)
			spells[indexCurrent]->upgrade();
		upgradeCount = 0;
	}
}


void PlayerHand::addSpell(SpellType type)
{
	if (spells.size() < maxSize)
	{
		switch (type)
		{
		case SpellType::DirectDamageSpell:
			spells.push_back(new DirectDamageSpell());
			break;
		case SpellType::AreaDamageSpell:
			spells.push_back(new AreaDamageSpell());
			break;
		case SpellType::TrapSpell:
			spells.push_back(new TrapSpell());
			break;
		case SpellType::AllySpell:
			spells.push_back(new AllySpell());
			break;
		case SpellType::UpgradeSpell:
			spells.push_back(new UpgradeSpell());
			break;
		default:
			break;
		}
	}
}


void PlayerHand::upgradeSpell(int count)
{
	upgradeCount = count;
}


bool PlayerHand::useSpell(ISpellContext& context, const Point& destination)
{
	if (indexCurrent < spells.size())
	{
		bool value = spells[indexCurrent]->use(context, pos, destination);
		if (value)
		{
			/* Удаление заклинания, если оно использовано.*/
			delete spells[indexCurrent];
			spells.erase(spells.begin() + indexCurrent);
			indexCurrent = -1;
			return true;
		}
	}
	return false;
}


PlayerHand::~PlayerHand()
{
	for (auto* spell : spells)
	{
		if (spell != nullptr)
			delete spell;
	}
}

