#include "TrapSpell.h"


TrapSpell::TrapSpell(int damage, std::string name)
	: damage(damage), minDamage(damage), name(name) { }


void TrapSpell::to_json(nlohmann::json& js) const
{
	js["type"] = SpellType::TrapSpell;
	js["name"] = name;
	js["damage"] = damage;
	js["minDamage"] = minDamage;
	js["level"] = level;
}


void TrapSpell::from_json(const nlohmann::json& js)
{
	if (js.contains("name"))
		name = js["name"];
	else
		throw std::runtime_error(u8"Ошибка считывания name");

	if (js.contains("damage"))
		damage = js["damage"];
	else
		throw std::runtime_error(u8"Ошибка считывания damage");

	if (js.contains("minDamage"))
		minDamage = js["minDamage"];
	else
		throw std::runtime_error(u8"Ошибка считывания minDamage");

	if (js.contains("level"))
		level = js["level"];
	else
		throw std::runtime_error(u8"Ошибка считывания level");
}


std::string TrapSpell::getName() const
{
	return name;
}


std::string TrapSpell::getInfo() const
{
	std::string info;
	info += "About: spawn trap at the point\n";
	info += "Spell level: " + std::to_string(level) + "\n";
	info += "Damage: " + std::to_string(damage) + "\n";
	return info;
}


int TrapSpell::getLevel() const
{
	return level;
}


bool TrapSpell::use(ISpellContext& context, const Point& usePos, const Point& destination)
{
	if (usePos == destination)
		return false;

	bool value = context.spawnTrap(destination);
	if (!value)
		return false;

	return true;
}


void TrapSpell::upgrade()
{
	level = level < 3 ? level + 1 : level;
	switch (level)
	{
	case 1:
		damage = minDamage + 5;
		break;
	case 2:
		damage = minDamage + 10;
		break;
	case 3:
		damage = minDamage + 15;
		break;
	default:
		break;
	}
}


Spell* TrapSpell::getCopy() const
{
	TrapSpell* newSpell = new TrapSpell(damage, name);
	newSpell->minDamage = this->minDamage;
	newSpell->level = this->level;
	return newSpell;
}