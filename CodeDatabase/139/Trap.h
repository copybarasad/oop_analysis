#pragma once
#include "DataTypes/Point.h"
#include "JsonLib/json.hpp"


class Trap
{
	int damage;
	bool used;
	Point pos;
public:
	Trap(const Point& pos, int damage = 10);
	void to_json(nlohmann::json& js) const;
	void from_json(const nlohmann::json& js);
	void changeUsed();
	bool isUsed() const;
	const Point& getPosition() const;
	int getDamage() const;
};