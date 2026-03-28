#pragma once
#include "Coords.h"
#include "Object.h"

class GameArea;

class Building : public Object { 
protected:
	int step = 0;
public:
	Building(double h, Coords crd) : Object(h, crd) {}
	virtual ~Building() = default;
	virtual void do_turn(GameArea& gamearea) = 0;
};