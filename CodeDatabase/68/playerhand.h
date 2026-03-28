#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include "spell.h"
#include <vector>
#include <memory>

class Player;

class PlayerHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;
    
public:
    PlayerHand(int maxSize = 5);
    
    PlayerHand(const PlayerHand&) = delete;
    PlayerHand& operator=(const PlayerHand&) = delete;
    PlayerHand(PlayerHand&&) = default;
    PlayerHand& operator=(PlayerHand&&) = default;
    
    bool addSpell(std::unique_ptr<Spell> spell);
    void removeSpell(int index);
    Spell* getSpell(int index) const;
    void gainRandomSpell();
    
    // NEW: Point-based spell purchase
    bool buySpell(Player* player, int spellCost = 15);
    
    int getSpellCount() const;
    int getMaxSize() const;
    bool isFull() const;
    void displayHand() const;

    std::vector<std::unique_ptr<Spell>>& getAllSpells() {
        return spells;
    }
    const std::vector<std::unique_ptr<Spell>>& getAllSpells() const{
        return spells;
    } 
};

#endif