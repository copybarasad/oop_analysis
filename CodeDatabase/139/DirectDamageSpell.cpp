#include "DirectDamageSpell.h"


DirectDamageSpell::DirectDamageSpell(int damage, int radius, int waitSteps, std::string name)
	: damage(damage), radius(radius), minRadius(radius), name(name), waitSteps(waitSteps), leftSteps(waitSteps) {}


void DirectDamageSpell::to_json(nlohmann::json& js) const
{
	js["type"] = SpellType::DirectDamageSpell;
	js["name"] = name;
	js["damage"] = damage;
	js["minRadius"] = minRadius;
	js["radius"] = radius;
	js["waitSteps"] = waitSteps;
	js["leftSteps"] = leftSteps;
	js["level"] = level;
}


void DirectDamageSpell::from_json(const nlohmann::json& js)
{
	if (js.contains("name"))
		name = js["name"];
	else
		throw std::runtime_error(u8"Ошибка считывания name");

	if (js.contains("damage"))
		damage = js["damage"];
	else
		throw std::runtime_error(u8"Ошибка считывания damage");

	if (js.contains("minRadius"))
		minRadius = js["minRadius"];
	else
		throw std::runtime_error(u8"Ошибка считывания minRadius");

	if (js.contains("radius"))
		radius = js["radius"];
	else
		throw std::runtime_error(u8"Ошибка считывания radius");

	if (js.contains("waitSteps"))
		waitSteps = js["waitSteps"];
	else
		throw std::runtime_error(u8"Ошибка считывания waitSteps");

	if (js.contains("leftSteps"))
		leftSteps = js["leftSteps"];
	else
		throw std::runtime_error(u8"Ошибка считывания leftSteps");

	if (js.contains("level"))
		level = js["level"];
	else
		throw std::runtime_error(u8"Ошибка считывания level");
}


std::string DirectDamageSpell::getName() const
{
	return name;
}


std::string DirectDamageSpell::getInfo() const
{
	std::string info;
	info += "About: direct damage to the enemies and tower\n";
	info += "Spell level: " + std::to_string(level) + "\n";
	info += "Radius: " + std::to_string(radius) + "\n";
	info += "Damage: " + std::to_string(damage) + "\n";
	return info;
}


int DirectDamageSpell::getLevel() const
{
	return level;
}


bool DirectDamageSpell::use(ISpellContext& context, const Point& usePos, const Point& destination)
{
	if (usePos == destination)
		return false;

	int left = usePos.X() - radius;
	int right = usePos.X() + radius;
	int up = usePos.Y() - radius;
	int down = usePos.Y() + radius;

	if (!(left <= destination.X() && destination.X() <= right && up <= destination.Y() && destination.Y() <= down))
		return false;

	auto* player = context.getEntityAt(destination);
	if (player != nullptr)
	{
		if (leftSteps <= 0)
			leftSteps = waitSteps;

		if (waitSteps == leftSteps)
			player->takeDamage(damage);
		--leftSteps;
		return true;
	}

	auto* enemy = context.getEnemyAt(destination);
	auto* tower = context.getTowerAt(destination);
	
	if (enemy == nullptr && tower == nullptr)
		return false;

	if (enemy != nullptr)
		enemy->takeDamage(damage);
	
	if (tower != nullptr)
		tower->takeDamage(damage);

	return true;
}


void DirectDamageSpell::upgrade()
{
	level = level < 3 ? level + 1 : level;
	switch (level)
	{
	case 1:
		radius = minRadius;
		break;
	case 2:
		radius = minRadius + 2;
		break;
	case 3:
		radius = minRadius + 4;
		break;
	default:
		break;
	}
}


Spell* DirectDamageSpell::getCopy() const
{
	DirectDamageSpell* newSpell = new DirectDamageSpell(damage, radius, waitSteps, name);
	newSpell->minRadius = this->minRadius;
	newSpell->leftSteps = this->leftSteps;
	newSpell->level = this->level;
	return newSpell;
}