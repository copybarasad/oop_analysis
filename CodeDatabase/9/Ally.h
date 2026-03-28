#ifndef ALLY_H
#define ALLY_H

#include "Character.h"
#include <utility>

class Ally : public Character{
public:
    Ally(int hp, int damage, std::pair<int,int> pos);

    bool shouldSkipTurn();
    void setSkipNextTurn(bool v);

private:
    bool skipNextTurn_;
};

#endif