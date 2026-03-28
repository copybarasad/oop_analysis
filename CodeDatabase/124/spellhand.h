#ifndef SPELLHAND_H
#define SPELLHAND_H

#include <vector>
#include <memory>
#include <random>
 
class SpellCard;
class SpellHand {
private:
    std::vector<std::shared_ptr<SpellCard>> spells;
    int maxSize;
    int selectedIndex;
    std::mt19937 rng;

public:
    SpellHand(int maxSpells = 3);
    
    bool addSpell(std::shared_ptr<SpellCard> spell);
    void selectNext();
    void selectPrevious();
    std::shared_ptr<SpellCard> getSelectedSpell();
    const std::shared_ptr<SpellCard> getSelectedSpell() const;
    const std::vector<std::shared_ptr<SpellCard>>& getSpells() const;
    
     
    bool removeRandomSpell();
     
    void copySpellsFrom(const SpellHand& other);
    int getSelectedIndex() const;
    int getSize() const;
    int getMaxSize() const;
    bool isFull() const;
    std::shared_ptr<SpellCard> generateRandomSpell();
    bool addRandomSpell();
    
    void updateCooldowns();


    bool addSpellByName(const std::string& spellName);  
    void clear();   
    std::vector<std::string> getSpellNames() const; 
};
#endif