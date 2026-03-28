#pragma once
#include <vector>
#include <memory>
#include "Spell.h"

class Hand {
public:
    Hand();
    ~Hand();

    void addSpell(Spell* spell);
    const Spell* getSpell(int index) const;
    Spell* getSpellMutable(int index);
    
    Spell* findSpell(SpellType type);

    int getSize() const;
    void nextSpell();
    void prevSpell();
    int getSelectedIndex() const;
    
    bool isFull() const;
    bool upgradeRandomSpell();

    void removeRandomSpell();
    void clear();
    const std::vector<Spell*>& getAllSpells() const { return m_spells; }

private:
    std::vector<Spell*> m_spells;
    int m_selectedIndex;
    static constexpr int MAX_HAND_SIZE = 5;
};