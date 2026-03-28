#ifndef IMPROVESPELL_H
#define IMPROVESPELL_H

#include "ISpell.h"

class ImproveSpell : public ISpell {
public:
    bool cast(int, Entity*, ISpellField*);
    bool getHasTarget() { return false; }
};

#endif



