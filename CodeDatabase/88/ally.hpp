#ifndef ALLY_HPP
#define ALLY_HPP

#include "character.hpp"

class Ally: public Character{
    char icon = 'A';
public:
    Ally(int startHealth, int startDamage, int startX, int startY)
        : Character(startHealth, startDamage, startX, startY) {}
    char getIcon() override { return icon; }
};

#endif