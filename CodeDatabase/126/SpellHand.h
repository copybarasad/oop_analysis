#ifndef SPELLHAND_H
#define SPELLHAND_H

#include <vector>
#include <memory>
#include "Spell.h"

class SpellHand {
    std::vector<std::shared_ptr<Spell>> spells;
    int maxSize;
    
public:
    SpellHand(int size = 3);
    
    void addSpell(std::shared_ptr<Spell> spell);
    void addRandomSpell();
    bool castSpell(int index, const Position& casterPos, const Position& targetPos, 
                   GameField& field, std::vector<std::shared_ptr<GameObject>>& objects);
    void displaySpells() const;
    
    size_t getSpellCount() const;
    int getMaxSize() const;
    
    void removeRandomSpells(int count);
    
    std::string serialize() const;
    void deserialize(std::istream& stream);
};

#endif