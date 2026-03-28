#ifndef ISPELL_H
#define ISPELL_H

#include "../Characters/Character.h"

class GameState;

class ISpell{
public:
    virtual bool cast(GameState& gameState) = 0;
    virtual void upgrade(int power) = 0;
    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
};

#endif