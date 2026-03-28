#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "ISpell.h"

class TrapSpell : public ISpell {
public:
    bool cast(int, Entity* target, ISpellField* field);
    bool getHasTarget() { return false; }
};

#endif
