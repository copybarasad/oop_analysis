#ifndef PLAYERHAND_H
#define PLAYERHAND_H

#include <vector>
#include <algorithm>
#include <stdexcept>
#include "CommonSpell.h"

class PlayerHand {
private:
    std::vector<CommonSpell> spells;
public:
    PlayerHand();
    explicit PlayerHand(const std::vector<CommonSpell>& initialSpells);

    void addSpell(const CommonSpell& spell);
    bool removeSpell(const std::string& spellName);
    bool removeSpell(int index);
    void clearHand();

    CommonSpell& getSpell(int index);
    const CommonSpell& getSpell(int index) const;
    std::vector<CommonSpell> getAllSpells() const;
};

#endif
