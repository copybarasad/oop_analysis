#pragma once
#include "Coords.h"
#include <string>
#include "GameError.h"


class Object {
protected:
	double health;
	Coords coords;
public:
	Object(double h, Coords c) : health(h), coords(c) {}
	virtual ~Object() = default;
	Coords get_coords();
	double get_health();
	void set_coords(int x, int y);
	void take_damage(double input_damage);
	void set_health(double h) { health = h;  };
};