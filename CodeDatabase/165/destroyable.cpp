#include "destroyable.h"
void destroyable::setHP(int HP)
{
    this->HP = HP;
}

void destroyable::setMapPosition(size_t x, size_t y)
{
    this->mapPosition.x = x;
    this->mapPosition.y = y;
}

void destroyable::setId(size_t id)
{
    this->id = id;
}

int destroyable::getHP()
{
    return this->HP;
}

coords destroyable::getMapPosition()
{
    return this->mapPosition;
}

size_t destroyable::getId()
{
    return this->id;
}
