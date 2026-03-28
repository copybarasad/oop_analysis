#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include <vector>
#include <memory>

class SpellHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    size_t maxSize;
    int enemiesDefeated = 0;
    int enemiesForNewSpell = 2;

public:
    explicit SpellHand(size_t handSize);
    
    SpellHand(const SpellHand& other);                  
    SpellHand(SpellHand&& other) = default;           
    SpellHand& operator=(const SpellHand& other);        
    SpellHand& operator=(SpellHand&& other) = default;  
    ~SpellHand() = default;                             
    
    bool addSpell(std::unique_ptr<Spell> spell);
    bool castSpell(size_t index, const Position& target, GameState& gameState, const Player& caster);
    void removeSpell(size_t index);

    size_t getSpellCount() const {
        return spells.size();
    }
    size_t getMaxSize() const {
        return maxSize;
    }
    const Spell* getSpell(size_t index) const;

    void onEnemyDefeated();
    bool canGetNewSpell() const;
    void resetEnemyCounter();
    
    bool tryLearnNewSpell();

    void displaySpells() const;
    
    std::pair<int, int> getSpellProgress() const {
        return {enemiesDefeated, enemiesForNewSpell};
    }
};

#endif