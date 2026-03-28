#ifndef HAND
#define HAND

#include "Spells/SquareDamageSpell.h"
#include "Spells/DirectDamageSpell.h"
#include <vector>

class Hand
{
private:
    int mMaxSize;
    int mSelection;
    std::vector<Spell *> mSpellVector;

public:
    Hand(int spellCap = 0);

    Hand(const Hand &h);

    void addSpell();

    void addSpecificSpell(Spell *sp);

    void deleteSpell(int i);

    Spell *getSpell(int i);

    int getSize() const;

    Hand &operator=(const Hand &h);

    Hand &operator=(Hand &&h);

    void setSelection(int sel);

    int getSelection() const;

    ~Hand();
};

#endif