#ifndef HAND_H
#define HAND_H

#include <vector>
#include <memory>
#include "spells/ISpell.h"

class Hand {
private:
    std::size_t maxSpellsCount;
    std::vector<SpellType> spells;
    int pendingBoostMultiplier;
    bool spellCastDelayActive = false;
public:
    Hand(std::size_t maxSpellsCount) :
    maxSpellsCount(maxSpellsCount), pendingBoostMultiplier(1) {}

    int getPendingBoostMultiplier() const { return pendingBoostMultiplier; }
    void setPendingBoostMultiplier(int pendingBoostMultiplier);

    bool getSpellCastDelayActive() const { return spellCastDelayActive; }
    void setSpellCastDelayActive(bool spellCastDelayActive);

    const std::vector<SpellType>& getSpells() const { return spells; }
    bool addSpell(SpellType spell);
    void removeSpell(std::size_t index);
    
    std::size_t getMaxSpellsCount() const { return maxSpellsCount; }
    void setMaxSpellsCount(std::size_t count) { maxSpellsCount = count; }
};

#endif
