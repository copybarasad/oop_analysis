#include "AreaDamageSpell.h"


AreaDamageSpell::AreaDamageSpell(int damage, int radius, std::string name)
	: damage(damage), radius(radius), name(name) { }


void AreaDamageSpell::to_json(nlohmann::json& js) const
{
	js["type"] = SpellType::AreaDamageSpell;
	js["name"] = name;
	js["damage"] = damage;
	js["radius"] = radius;
	js["area"] = area;
	js["level"] = level;
}


void AreaDamageSpell::from_json(const nlohmann::json& js)
{
	if (js.contains("name"))
		name = js["name"];
	else
		throw std::runtime_error(u8"Ошибка считывания name");

	if (js.contains("damage"))
		damage = js["damage"];
	else
		throw std::runtime_error(u8"Ошибка считывания damage");

	if (js.contains("radius"))
		radius = js["radius"];
	else
		throw std::runtime_error(u8"Ошибка считывания radius");

	if (js.contains("area"))
		area = js["area"];
	else
		throw std::runtime_error(u8"Ошибка считывания area");

	if (js.contains("level"))
		level = js["level"];
	else
		throw std::runtime_error(u8"Ошибка считывания level");
}


std::string AreaDamageSpell::getName() const
{
	return name;
}


std::string AreaDamageSpell::getInfo() const
{
	std::string info;
	info += "About: damage enemies and the tower\n";
	info += "Spell level: " + std::to_string(level) + "\n";
	info += "Radius: " + std::to_string(radius) + "\n";
	info += "Area: " + std::to_string(area) + "\n";
	info += "Damage: " + std::to_string(damage) + "\n";
	return info;
}


int AreaDamageSpell::getLevel() const
{
	return level;
}


bool AreaDamageSpell::use(ISpellContext& context, const Point& usePos, const Point& destination)
{
	if (usePos == destination)
		return false;

	int left = usePos.X() - radius;
	int right = usePos.X() + radius;
	int up = usePos.Y() - radius;
	int down = usePos.Y() + radius;

	if (!(left <= destination.X() && destination.X() <= right && up <= destination.Y() && destination.Y() <= down))
		return false;


	for (auto* enemy : context.getEnemiesAt(destination, area))
		enemy->takeDamage(damage);

	auto* tower = context.getTowerAt(destination, area);
	if (tower != nullptr)
		tower->takeDamage(damage);

	return true;
}


void AreaDamageSpell::upgrade()
{
	level = level < 3 ? level + 1 : level;
	switch (level)
	{
	case 1:
		area = 2;
		break;
	case 2:
		area = 3;
		break;
	case 3:
		area = 4;
		break;
	default:
		break;
	}
}


Spell* AreaDamageSpell::getCopy() const
{
	AreaDamageSpell* newSpell = new AreaDamageSpell(damage, radius, name);
	newSpell->area = this->area;
	newSpell->level = this->level;
	return newSpell;
}