#include "../Headers/Units.h"

Cell::Cell() : _x{ -1 }, _y{ -1 }
{
}

Cell::Cell(int x, int y) : _x{ x }, _y{ y }
{
}

bool Cell::operator==(const Cell& other) const
{
    return _x == other._x && _y == other._y;
}

bool Cell::operator!=(const Cell& other) const
{
    return !(*this == other);
}