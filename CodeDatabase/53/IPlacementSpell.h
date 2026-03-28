#ifndef IPLACEMENTSPELL_H
#define IPLACEMENTSPELL_H

#include "ISpell.h"

class Player;

class IPlacementSpell : public virtual ISpell {
public:
    virtual ~IPlacementSpell() = default;
    virtual bool isValidPlacement(int x, int y, const Player& player) const = 0;
};

#endif