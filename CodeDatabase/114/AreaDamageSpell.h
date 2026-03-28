#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "ISpell.h"
#include <vector>

class Player;

class AreaDamageSpell : public ISpell {
public:
    bool cast(int pendingBoost, Entity* target, ISpellField* field);
    bool getHasTarget() { return true; }
};

#endif
