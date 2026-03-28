#ifndef ISPELL_H
#define ISPELL_H

#include "Position.h"
#include "SpellSaveData.h"

class GameController;

class ISpell
{
private:
    bool improved = false;
public:
    virtual ~ISpell() = default;
    virtual bool apply(GameController &controller, const Position &casterPosition, const Position &targetPos) = 0;
    virtual const char *name() const = 0;
    virtual SpellType getType() const = 0;
    virtual void getParameters(int& damage, int& range, int& width, int& height, int& count, int& power) const = 0;
    bool isImproved() const { return improved; }
    void setImproved(bool value) { improved = value; }
};

#endif