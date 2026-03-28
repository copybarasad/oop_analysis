#pragma once
#include "Object.h"

class Trap : public Object {
private:
	double damage;
public:
	Trap(Coords c, double d) : Object(1, c), damage(d) {}  //1 в поле health, значит, что ловушка еще не использовалась
	double get_damage()
	{
		return damage;
	}
};