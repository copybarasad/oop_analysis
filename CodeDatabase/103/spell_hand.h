#pragma once
#include <vector>
#include "ispell.h"
#include <iostream>

class SpellHand {
private:
    std::vector<ISpell*> spells;
    int maxSize;

public:
    SpellHand(int size = 3);
    ~SpellHand();

    int getMaxSize() const;
    void addSpell(ISpell* spell);
    void addSpellByName(const std::string& name,int damage,  int cost, int raduis);
    const std::vector<ISpell*>& getSpells() const;
    ISpell* getSpell(int index) const;
    void removeSpell(int index);
    std::vector<std::tuple<std::string, int, int, int>> getSpellsData() const;
};