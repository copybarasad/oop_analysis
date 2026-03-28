#include "cell.h"
#include <iostream>

Cell::Cell() {
	battler_ = nullptr;
}

BasicBattler* Cell::getBattler() const {
	return battler_;
}
void Cell::setBattler(BasicBattler* new_battler) {
	battler_ = new_battler;
}

bool Cell::isFree() {
	return battler_ == nullptr;
}
void Cell::clean() {
	battler_ = nullptr;
}
