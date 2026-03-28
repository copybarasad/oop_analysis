#ifndef SPELLCARDFACTORY_H
#define SPELLCARDFACTORY_H

#include <memory>
#include "constants.h"
#include "ispellcard.h"

class SpellCardFactory
{
public:
    SpellCardFactory() = default;
    ~SpellCardFactory() = default;

    const std::unordered_map<CardType, int>& getCardsCosts() const { return m_cardsCosts; }
    int getCostCard(CardType type) const;
    CardType getRandomType() const;
    std::unique_ptr<ISpellCard> createSpellCard(CardType type);

private:
    static std::unordered_map<CardType, int> m_cardsCosts;
};

#endif // SPELLCARDFACTORY_H
