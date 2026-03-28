#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include <vector>
#include <memory>
#include <iostream>

class SpellHand {
private:
    std::vector<std::shared_ptr<Spell>> spells;
    size_t maxSize;
    std::shared_ptr<Spell> nextEnhancement;
    
public:
    SpellHand(size_t maxSize = 5);
    
    bool addSpell(std::shared_ptr<Spell> spell);
    bool removeSpell(size_t index);
    std::shared_ptr<Spell> getSpell(size_t index) const;
    size_t getSpellCount() const;
    size_t getMaxSize() const;
    
    void addRandomSpell();
    bool castSpell(size_t index, const Position& target, GameField& field, const Position& casterPos);
    
    void setNextEnhancement(std::shared_ptr<Spell> enhancement);
    void applyEnhancement(std::shared_ptr<Spell> spell);
    
    void displaySpells() const;
    
    // Новые методы для сохранения/загрузки
    void save(std::ostream& file) const;
    void load(std::istream& file);
};

#endif
