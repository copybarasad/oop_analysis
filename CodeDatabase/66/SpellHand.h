#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSpells;

public:
    SpellHand(int maxSpells = 5);
    
    void addSpell(std::unique_ptr<Spell> spell);
    void removeSpell(int index);
    bool hasSpell(int index) const;
    Spell* getSpell(int index);
    const Spell* getSpell(int index) const;
    int getSpellCount() const;
    int getMaxSpells() const;
    
    void addRandomSpell();
    
    const std::vector<std::unique_ptr<Spell>>& getSpells() const;
};

#endif
