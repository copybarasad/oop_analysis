#ifndef HAND_H
#define HAND_H

#include "spell.h"
#include "GameTypes.h"  
#include <vector>
#include <memory>
#include <string>

class Hand {
private:
    int size_hand;
    std::vector<std::unique_ptr<ISpell>> spells;
    
public:
    Hand(int size_hand);
    Hand(const Hand& other);                  
    Hand& operator=(const Hand& other); 
    
    const std::vector<std::unique_ptr<ISpell>>& GetSpells() const;
    
    bool ShowSpells() const;
    SpellResult UseSpell(int index_spell, std::vector<Enemy>& enemies, 
                        int playerX, int playerY, 
                        const std::vector<std::pair<int,int>>& enemyPositions,
                        int targetX, int targetY);
    bool AddSpell(std::unique_ptr<ISpell> spell);
    bool AddRandomSpell();
    std::string RemoveHalfSpells();
    
    void ClearSpells();
    void AddLoadedSpell(std::unique_ptr<ISpell> spell);
    std::string GetSpellsInfo() const;
    
    bool IsFull() const { return static_cast<int>(spells.size()) >= size_hand; }
    int GetSize() const { return size_hand; }
    int GetSpellCount() const { return static_cast<int>(spells.size()); }
};

#endif