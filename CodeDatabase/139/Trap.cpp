#include "Trap.h"


Trap::Trap(const Point& pos, int damage)
	: pos(pos), damage(damage), used(false) { }


void Trap::to_json(nlohmann::json& js) const
{
	js["position"] =
	{
		{"x", pos.X()},
		{"y", pos.Y()}
	};

	js["damage"] = damage;
	js["used"] = used;
}


void Trap::from_json(const nlohmann::json& js)
{
	if (js.contains("damage"))
		damage = js["damage"];
	else
		throw std::runtime_error(u8"Ошибка считывания damage");

	if (js.contains("used"))
		used = js["used"];
	else
		throw std::runtime_error(u8"Ошибка считывания used");

	if (js.contains("position") && js["position"].contains("x") && js["position"].contains("y"))
	{
		pos = Point(
			js["position"]["x"],
			js["position"]["y"]
		);
	}
	else
		throw std::runtime_error(u8"Ошибка считывания position");
}


void Trap::changeUsed()
{
	used = !used;
}


bool Trap::isUsed() const
{
	return used;
}


const Point& Trap::getPosition() const
{
	return pos;
}


int Trap::getDamage() const
{
	return damage;
}