#pragma once

#include "Spell.h"
#include <vector>
#include <memory>

class PlayerHand {
private:
    std::vector<std::unique_ptr<Spell>> spells;
    int maxSize;
    int selectedIndex;
    int killCount;
    int killsForNewSpell;
    EnhancementContext enhancement;

public:
    PlayerHand(int maxSize = 5, int killsForNewSpell = 3);

    bool addSpell(std::unique_ptr<Spell> spell);

    bool removeSpell(int index);

    Spell* getSelectedSpell() const;

    void selectNext();

    void selectPrevious();

    bool selectSpell(int index);

    int getSelectedIndex() const;

    int getSpellCount() const;

    int getMaxSize() const;

    bool hasSpace() const;

    const Spell* getSpell(int index) const;

    void addRandomSpell();

    void registerKill();

    bool checkAndRewardSpell();

    EnhancementContext& getEnhancement();

    void resetEnhancement();

    int getKillCount() const;

    int getKillsUntilNextSpell() const;
    
    void clearAllSpells();
    void setKillCount(int count);
    void setSelectedIndex(int index);
};
