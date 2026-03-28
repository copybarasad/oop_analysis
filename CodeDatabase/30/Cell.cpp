#include "../include/Cell.h"

Cell::Cell() : passable_(true), slowing_(false) {}

Cell::Cell(bool passable, bool slowing)
	: passable_(passable), slowing_(slowing), trapDamage_(0) {}

bool Cell::isPassable() const { return passable_; }
void Cell::setPassable(bool p) { passable_ = p; }

void Cell::placeTrap(int dmg) {
	trapDamage_ = dmg > 0 ? dmg : 0;
}

int Cell::triggerTrap() {
	if (trapDamage_ <= 0) return 0;
	int dmg = trapDamage_;
	trapDamage_ = 0;
	return dmg;
}