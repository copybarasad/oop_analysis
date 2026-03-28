#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "ISpell.h"

class SummonSpell : public ISpell {
public:
    bool cast(int pendingBoost, Entity*, ISpellField* field);
    bool getHasTarget() { return false; }
};

#endif



