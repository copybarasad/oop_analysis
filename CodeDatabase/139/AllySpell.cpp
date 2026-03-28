#include "AllySpell.h"


AllySpell::AllySpell(int health, int damage, std::string name) 
	: health(health), damage(damage), name(name) { }


void AllySpell::to_json(nlohmann::json& js) const
{
	js["type"] = SpellType::AllySpell;
	js["name"] = name;
	js["health"] = health;
	js["damage"] = damage;
	js["radius"] = radius;
	js["alliesCount"] = alliesCount;
	js["level"] = level;
}


void AllySpell::from_json(const nlohmann::json& js)
{
	if (js.contains("name"))
		name = js["name"];
	else
		throw std::runtime_error(u8"Ошибка считывания name");

	if (js.contains("health"))
		health = js["health"];
	else
		throw std::runtime_error(u8"Ошибка считывания health");

	if (js.contains("damage"))
		damage = js["damage"];
	else
		throw std::runtime_error(u8"Ошибка считывания damage");

	if (js.contains("radius"))
		radius = js["radius"];
	else
		throw std::runtime_error(u8"Ошибка считывания radius");

	if (js.contains("alliesCount"))
		alliesCount = js["alliesCount"];
	else
		throw std::runtime_error(u8"Ошибка считывания alliesCount");

	if (js.contains("level"))
		level = js["level"];
	else
		throw std::runtime_error(u8"Ошибка считывания level");
}


std::string AllySpell::getName() const
{
	return name;
}


std::string AllySpell::getInfo() const
{
	std::string info;
	info += "About: Spawn allies around player\n";
	info += "Spell level: " + std::to_string(level) + "\n";
	info += "Radius: " + std::to_string(radius) + "\n";
	info += "Damage: " + std::to_string(damage) + "\n";
	info += "Allies count: " + std::to_string(alliesCount) + "\n";
	return info;
}


int AllySpell::getLevel() const
{
	return level;
}


bool AllySpell::use(ISpellContext& context, const Point& usePos, const Point& destination)
{
	if (usePos != destination)
		return false;

	context.spawnAlly(health, damage, alliesCount, destination);
	return true;
}


void AllySpell::upgrade()
{
	level = level < 3 ? level + 1 : level;
	switch (level)
	{
	case 1:
		alliesCount = 1;
		break;
	case 2:
		alliesCount = 2;
		break;
	case 3:
		alliesCount = 3;
		break;
	default:
		break;
	}
}


Spell* AllySpell::getCopy() const 
{
	AllySpell* newSpell = new AllySpell(health, damage, name);
	newSpell->alliesCount = this->alliesCount;
	newSpell->level = this->level;
	newSpell->radius = this->radius;
	return newSpell;
}