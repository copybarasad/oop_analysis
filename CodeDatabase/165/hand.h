#ifndef HANDH
#define HANDH
#include "spell.h"
#include <vector>
class hero;

class hand{
    std::vector<spell*> spellList;
    size_t spellListLimit;
public:
    hand();
    std::vector<spell*>& getSpellList();
    size_t getSpellListLimit();
    void setSpellListLimit(size_t);
    bool craft(hero*);
    bool addSpell(size_t, size_t);
    void freeHand(size_t);
};
#endif