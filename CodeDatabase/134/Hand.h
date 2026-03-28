#pragma once
#include <vector>
#include "Spell.h"
#include "AreaDamageSpell.h"
#include "DirectDamageSpell.h"

class Hand {
public:
    Hand(int maxSize);

    void AddSpell(Spell* spell);
    Spell* GetSpell(int index) const;
    int GetSize() const;
    int GetMaxSize() const;
    void UseSpell(int index, Player& player, GameField& field, int x, int y) const;

private:
    int maxSize;
    std::vector<Spell*> spells;
};
