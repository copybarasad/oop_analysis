#ifndef ISPELL_H
#define ISPELL_H

#include <string>
#include "Point.h"
#include "GameTypes.h"

class Game;
class Entity;

class ISpell {
public:
    virtual ~ISpell() = default;
    virtual bool cast(Game& game, Entity& caster, const Point& target) = 0;
    virtual std::string getName() const = 0;
    virtual int getCost() const = 0;
    virtual SpellType getType() const = 0;
};

#endif