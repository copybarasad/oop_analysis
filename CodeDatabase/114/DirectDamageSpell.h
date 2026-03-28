#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "ISpell.h"

class DirectDamageSpell : public ISpell {
public:
    bool cast(int pendingBoost, Entity* target, ISpellField* field);
    bool getHasTarget() { return true; }
};

#endif
