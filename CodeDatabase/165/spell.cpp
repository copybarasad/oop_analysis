#include "spell.h"

void spell::setName(std::string name)
{
    this->name = name;
}

void spell::setRange(size_t range)
{
    this->range = range;
}

std::string spell::getName()
{
    return this->name;
}

size_t spell::getRange()
{
    return this->range;
}

size_t spell::getRank()
{
    return this->rank;
}
