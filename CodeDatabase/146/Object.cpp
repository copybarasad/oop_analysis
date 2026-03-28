#include "Object.h"
#include "GameArea.h"
#include <cstdlib>
#include <random>


Coords Object::get_coords()
{
    return coords;
}

void Object::set_coords(int x, int y)
{
    coords.x = x;
    coords.y = y;
}

double Object::get_health()
{
    return health;
}

void Object::take_damage(double input_damage)
{
    health -= input_damage;
}
