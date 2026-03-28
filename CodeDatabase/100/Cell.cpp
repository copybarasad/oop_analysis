#include "Cell.h"

// Конструктор по умолчанию (исп. в конструкторе GameField)
Cell::Cell() : x(0), y(0), isPassable(true), isSlowing(false), hasTrap(false) { }

// Основной конструктор
Cell::Cell(int x, int y, bool passable, bool slowing, bool hastrap) :
        x(x), y(y), isPassable(passable), isSlowing(slowing), hasTrap(hastrap) { }

// Геттеры
bool Cell::getIsPassable() const { return isPassable; }

bool Cell::getIsSlowing() const { return isSlowing; }

bool Cell::getHasTrap() const { return hasTrap; }

// Сеттеры
void Cell::setIsPassable(bool passable) { isPassable = passable; } 

void Cell::setIsSlowing(bool slowing) { isSlowing = slowing; }

void Cell::setHasTrap(bool hastrap) { hasTrap = hastrap; }