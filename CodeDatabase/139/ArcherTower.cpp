#include "ArcherTower.h"


ArcherTower::ArcherTower(Point pos, int radius) : radius(radius), pos(pos)
{
	directSpell = new DirectDamageSpell(radius);
}


void ArcherTower::to_json(nlohmann::json& js) const
{
	js["position"] =
	{
		{"x", pos.X()},
		{"y", pos.Y()}
	};

	js["radius"] = radius;

	if (directSpell) {
		nlohmann::json jSpell;
		directSpell->to_json(jSpell);
		js["spell"] = jSpell;
	}
}


void ArcherTower::from_json(const nlohmann::json& js)
{
	if (js.contains("position") && js["position"].contains("x") && js["position"].contains("y"))
	{
		pos = Point(
			js["position"]["x"],
			js["position"]["y"]
		);
	}
	else
		throw std::runtime_error(u8"Ошибка считывания position");

	if (js.contains("radius"))
		radius = js["radius"];
	else
		throw std::runtime_error(u8"Ошибка считывания radius");

	if (directSpell)
		delete directSpell;

	directSpell = new DirectDamageSpell();

	if (js.contains("spell"))
	{
		try {
			directSpell->from_json(js["spell"]);
		}catch (...) {
			throw std::runtime_error(u8"Не удалось считать spell");
		}
	}
	else
		throw std::runtime_error(u8"Отсутствует поле spell");
}


void ArcherTower::setPoint(const Point& pos)
{
	this->pos = pos;
}


const Point& ArcherTower::getPosition() const
{
	return pos;
}


int ArcherTower::getRadius() const
{
	return radius;
}


void ArcherTower::useSpell(ISpellContext& context, const Point& destination)
{
	directSpell->use(context, pos, destination);
}


ArcherTower::~ArcherTower()
{
	if (directSpell != nullptr)
	{
		delete directSpell;
		directSpell = nullptr;
	}
}
