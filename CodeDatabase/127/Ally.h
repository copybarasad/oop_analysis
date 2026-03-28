// Ally.h
#ifndef ALLY_H
#define ALLY_H

#include "Unit.h"

class Ally : public Unit {
public:
    Ally(int x = 0, int y = 0, int hp = 50, int damage = 15);
    ~Ally() override = default;
};

#endif 