#include "cell.hpp"

Cell::Cell(bool isPass) : isPass(isPass) {}

bool Cell::get_isPass() const { return isPass; }

void Cell::setIsPassable(bool passable) { isPass = passable; }