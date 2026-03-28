#ifndef ISUMMONSPELL_H
#define ISUMMONSPELL_H

#include "ISpell.h"

class GameField;
class Player;

class ISummonSpell : public virtual ISpell {
public:
    virtual ~ISummonSpell() = default;
    virtual bool canSpawn(int x, int y, const Player& player, GameField& field) = 0;
    virtual int getMaxSpawns() const = 0;
    virtual int getCurrentSpawns() const = 0;
    virtual void setCurrentSpawns(int curSpawn) = 0;
};

#endif