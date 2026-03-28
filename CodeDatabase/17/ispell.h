#ifndef ISPELL_H
#define ISPELL_H

#include "position.h"
#include <string>

class GameMap;

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual bool cast(GameMap& map, Position casterPos, Position target) = 0;
    virtual std::string getName() const = 0;
    virtual int getRange(int lev) const = 0;
};

#endif