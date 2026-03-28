#ifndef HANDSPLAYER_H
#define HANDSPLAYER_H

#include <unordered_map>
#include "spellcardfactory.h"
#include "ispellconfig.h"

class HandsPlayer
{
public:
    HandsPlayer(int mana = CharacterConfig::Mana, int maxHandSize = 5);

    int& getMana() {return m_mana;}
    int& getCountCards(CardType type);
    ISpellCard* getCard(CardType type);
    CardType getSelectedSpell() { return m_selectedSpell; }
    bool inTargetMode() { return m_inTargetMode == true; }
    bool inUpgradeMode() { return m_inUpgradeMode == true; }
    void setSelectedSpell(CardType type) { m_selectedSpell = type; }
    void setUpgradeMode(bool mode) { m_inUpgradeMode = mode; }
    void setTargetMode(bool mode) { m_inTargetMode = mode; }
    int getUpgradeCardLevel(CardType type);

    void addMana(int mana);
    void upgradeRandomCard();
    bool buyCard(CardType type);
    void useCard(CardType type, ISpellConfig& config);
    bool hasCard(CardType type) const;
    void deleteRandomCards();
    void recalcTotalCount();

private:
    std::unordered_map<CardType, std::pair<int, std::unique_ptr<ISpellCard>>> m_cards;
    std::unique_ptr<SpellCardFactory> m_factory;
    int m_mana;
    int m_maxHandSize;
    int m_countCards;
    CardType m_selectedSpell{CardType::NONE};
    bool m_inTargetMode{false};
    bool m_inUpgradeMode{false};
};

#endif // HANDSPLAYER_H
