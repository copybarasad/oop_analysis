#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include "FireballSpell.h"
#include "ExplosionSpell.h"
#include "TrapSpell.h"
#include "InvisibilitySpell.h"
#include "SummonSpell.h"
#include "EnhanceSpell.h"
#include <vector>

class SpellHand {
private:
    std::vector<Spell*> spells;
    int maxSize;
    int selectedSpell;

public:
    SpellHand(int maxSize = 5);
    ~SpellHand();
    
    SpellHand(const SpellHand& other);
    SpellHand& operator=(const SpellHand& other);
    
    bool addSpell(Spell* spell);
    bool removeSpell(int index);
    Spell* getSpell(int index) const;
    int getSpellCount() const;
    int getMaxSize() const;
    
    void selectSpell(int index);
    Spell* getSelectedSpell() const;    
    
    static Spell* createRandomSpell();

    void clearSpells() {
        for (Spell* spell : spells) {
            delete spell;
        }
        spells.clear(); 
    }
};

#endif