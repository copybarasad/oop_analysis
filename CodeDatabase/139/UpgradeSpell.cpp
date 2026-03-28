#include "UpgradeSpell.h"


UpgradeSpell::UpgradeSpell(std::string name)
	: name(name) { }


void UpgradeSpell::to_json(nlohmann::json& js) const
{
	js["type"] = SpellType::UpgradeSpell;
	js["name"] = name;
	js["upgradeCount"] = upgradeCount;
	js["level"] = level;
}


void UpgradeSpell::from_json(const nlohmann::json& js)
{
	if (js.contains("name"))
		name = js["name"];
	else
		throw std::runtime_error(u8"Ошибка считывания name");

	if (js.contains("upgradeCount"))
		upgradeCount = js["upgradeCount"];
	else
		throw std::runtime_error(u8"Ошибка считывания upgradeCount");

	if (js.contains("level"))
		level = js["level"];
	else
		throw std::runtime_error(u8"Ошибка считывания level");
}


std::string UpgradeSpell::getName() const
{
	return name;
}


std::string UpgradeSpell::getInfo() const
{
	std::string info;
	info += "About: upgrade next taken spell\n";
	info += "Spell level: " + std::to_string(level) + "\n";
	info += "Level up: " + std::to_string(upgradeCount) + "\n";
	return info;
}



int UpgradeSpell::getLevel() const
{
	return level;
}


bool UpgradeSpell::use(ISpellContext& context, const Point& usePos, const Point& destination)
{
	if (usePos == destination)
	{
		context.upgradeSpell(upgradeCount);
		return true;
	}
	return false;
}


void UpgradeSpell::upgrade()
{
	level = level < 3 ? level + 1 : level;
	switch (level)
	{
	case 1:
		upgradeCount = 1;
		break;
	case 2:
		upgradeCount = 2;
		break;
	case 3:
		upgradeCount = 3;
		break;
	default:
		break;
	}
}


Spell* UpgradeSpell::getCopy() const
{
	UpgradeSpell* newSpell = new UpgradeSpell(name);
	newSpell->upgradeCount = this->upgradeCount;
	newSpell->level = this->level;
	return newSpell;
}