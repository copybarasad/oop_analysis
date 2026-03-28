#pragma once

#include "Coords.h"
#include <string>
#include "GameError.h"
#include "Object.h"

class GameArea;

class Human : public Object {
protected:
	double damage;
	bool stunned = false;
public:
	Human(double h, double d, Coords coords) : Object(h, coords), damage(d) {}
	virtual ~Human() = default;
	double get_damage();
	virtual bool attack(Coords enemy_coords, GameArea& gamearea) = 0;
	bool moving(std::string direction, GameArea& gamearea);
	double damage_calculation();
	void set_stunned(bool new_stunned);
	bool stunned_or_not();
};