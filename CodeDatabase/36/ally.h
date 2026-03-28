#ifndef ALLY_H
#define ALLY_H

#include "character.h"

class Ally : public Character {
public:
    Ally(int health = 3, int damage = 2);
};

#endif