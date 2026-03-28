#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "spell_card.h"
#include <vector>
#include <memory>

class SpellHand {
private:
    std::vector<std::unique_ptr<SpellCard>> spells;
    int maxSize;
    int enemiesDefeatedCount;
    static const int ENEMIES_FOR_NEW_SPELL = 3;

    void validateHandSize() const;

public:
    explicit SpellHand(int handSize = 5);

    bool removeRandomSpell();
    bool addSpell(std::unique_ptr<SpellCard> spell);
    bool removeSpell(int index);
    void castSpell(int index);
    SpellCard* getSpell(int index) const;
    int getSpellCount() const;
    int getMaxSize() const;
    bool isFull() const;
    
    void onEnemyDefeated();
    bool canBuySpell(int playerScore) const;
    void buySpell(int& playerScore);
    
    void initializeWithRandomSpell();
};

#endif