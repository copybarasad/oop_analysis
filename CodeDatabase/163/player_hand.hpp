#ifndef PLAYER_HAND_HPP
#define PLAYER_HAND_HPP

#include <vector>
#include <memory>
#include "spell_card.hpp"

class PlayerHand {
private:
    std::vector<std::unique_ptr<SpellCard>> spells;
    int maxHandSize;
    int enemiesDefeated;
    int spellPoints;
    static bool randomInitialized;
    
    static void initRandom();
    std::unique_ptr<SpellCard> createRandomSpell() const;

public:
    PlayerHand(int maxHandSize = 5);
    
    bool addSpell(std::unique_ptr<SpellCard> spell);
    bool removeSpell(int index);
    void displayHand() const;
    bool isFull() const;
    int getSize() const;
    int getMaxSize() const;
    int getSpellPoints() const;
    
    void enemyDefeated();
    void addSpellPoints(int points);
    bool canBuySpell() const;
    void buyRandomSpell();
    
    const SpellCard* getSpell(int index) const;
    SpellCard* getSpell(int index);
    const std::vector<std::unique_ptr<SpellCard>>& getSpells() const;
    std::vector<std::unique_ptr<SpellCard>>& getSpells();
};

#endif