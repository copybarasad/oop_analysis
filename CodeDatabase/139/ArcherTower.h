#pragma once
#include "DataTypes/Point.h"
#include "Spells/DirectDamageSpell.h"
#include "Interfaces/ISpellContext.h"
#include "JsonLib/json.hpp"


class ArcherTower
{
private:
	DirectDamageSpell* directSpell = nullptr;
	int radius;
	Point pos;
public:
	ArcherTower(Point pos, int radius = 6);
	void to_json(nlohmann::json& js) const;
	void from_json(const nlohmann::json& js);
	void setPoint(const Point& pos);
	const Point& getPosition() const;
	int getRadius() const;
	void useSpell(ISpellContext& context, const Point& destination);
	~ArcherTower();
};