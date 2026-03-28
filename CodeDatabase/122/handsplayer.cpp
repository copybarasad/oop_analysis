#include "handsplayer.h"

HandsPlayer::HandsPlayer(int mana, int maxHandSize) : m_factory(new SpellCardFactory()), m_mana(mana), m_maxHandSize(maxHandSize), m_countCards(0)
{
    for (const auto& [cardType, cost] : m_factory->getCardsCosts())
    {
        auto card = m_factory->createSpellCard(cardType);
        m_cards[cardType] = std::make_pair(0, std::move(card));
    }
}

ISpellCard *HandsPlayer::getCard(CardType type)
{
    return m_cards[type].second.get();
}

void HandsPlayer::deleteRandomCards()
{
    int halfCard = m_countCards / 2;
    while (m_countCards > halfCard)
    {
        CardType randomType = m_factory->getRandomType();
        m_cards[randomType].first = 0;
        --m_countCards;
    }
}

int& HandsPlayer::getCountCards(CardType type)
{
    if (type == CardType::NONE)
        throw std::runtime_error("Attempted to access NONE card");

    return m_cards[type].first;
}

int HandsPlayer::getUpgradeCardLevel(CardType type)
{
    return m_cards[type].second->getUpgradeLevel();
}

void HandsPlayer::addMana(int mana)
{
    m_mana += mana;
}

bool HandsPlayer::buyCard(CardType type)
{
    if (m_countCards >= m_maxHandSize)
    {
        return false;
    }

    int cost = m_factory->getCostCard(type);
    if (cost > m_mana) return false;
    m_mana -= cost;

    m_cards[type].first++;
    ++m_countCards;
    return true;
}

void HandsPlayer::useCard(CardType type, ISpellConfig& config)
{
    m_cards[type].second->useCard(config);
    if (m_cards[type].second->wasUsed())
    {
        m_cards[type].second->clearUpgrades();
        m_cards[type].first--;
        --m_countCards;
    }
}

bool HandsPlayer::hasCard(CardType type) const
{
    auto it = m_cards.find(type);
    return it != m_cards.end() && it->second.first;
}

void HandsPlayer::upgradeRandomCard()
{
    CardType randomType = m_factory->getRandomType();
    if (getCountCards(randomType) == 0)
    {
        m_cards[randomType].first++;
    }
    int currentLevel = m_cards[randomType].second->getUpgradeLevel();
    m_cards[randomType].second->upgrade(currentLevel + 1);
}

void HandsPlayer::recalcTotalCount()
{
    m_countCards = 0;
    for (auto& [type, pair] : m_cards)
        m_countCards += pair.first;
}
