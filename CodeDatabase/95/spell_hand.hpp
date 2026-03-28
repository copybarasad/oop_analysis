#ifndef SPELL_HAND_HPP
#define SPELL_HAND_HPP

#include <vector>
#include <memory>
#include "spell_card.hpp"

class SpellHand {
private:
    std::vector<SpellCard*> spells;
    size_t maxSize;

public:
    SpellHand(std::vector<SpellCard*> initialSpells = {});

    bool addSpell(SpellCard* spell);
    void removeSpell(size_t index);

    SpellCard* getSpell(size_t index);
    const SpellCard* getSpell(size_t index) const;

    void listSpells() const;

    size_t getSize() const;
    size_t getMaxSize() const;
};

#endif
