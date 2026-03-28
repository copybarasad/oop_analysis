#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <vector>
#include "entity.h"

enum SpellType
{
    AREA = 1,
    SINGLE = 2,
    TRAP = 3
};

struct CastContext
{
    int targetX;
    int targetY;
    Entity* gamer;

    CastContext(int targetX, int targetY, Entity* gamer)
        : targetX(targetX), targetY(targetY), gamer(gamer) {}
};

class ISpell
{
public:
    virtual ~ISpell() = default;
    virtual SpellType getTypeId() const = 0;
    virtual int getDmg() const = 0;
    virtual void setDmg(int dmg) = 0;
    virtual void cast(const CastContext &context) const = 0;
    virtual std::string getName() const = 0;
    virtual void upgrade() = 0;
};

#endif