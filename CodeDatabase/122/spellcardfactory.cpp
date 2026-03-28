#include "spellcardfactory.h"
#include "fireballcard.h"
#include "earthquakecard.h"
#include "summonarchercard.h"
#include "poisontrapcard.h"
#include "upgradecard.h"

int SpellCardFactory::getCostCard(CardType type) const
{
    auto it = m_cardsCosts.find(type);
    if (it == m_cardsCosts.end())
    {
        return false;
    }
    return it->second;
}

CardType SpellCardFactory::getRandomType() const
{
    std::uniform_int_distribution<size_t> die(0, m_cardsCosts.size() - 1);
    auto it = m_cardsCosts.begin();
    std::advance(it, die(MyRandom::Mersenne));
    return it->first;
}

std::unique_ptr<ISpellCard> SpellCardFactory::createSpellCard(CardType type)
{
    switch(type) {
        case CardType::FIREBALL:
        {
            return std::make_unique<FireBallCard>();
        }
        case CardType::EARTHQUAKE:
        {
            return std::make_unique<EarthQuakeCard>();
        }
        case CardType::POISON_TRAP:
        {
            return std::make_unique<PoisonTrapCard>();
        }
        case CardType::SUMMON_ARCHER:
        {
            return std::make_unique<SummonArcherCard>();
        }
        case CardType::UPGRADE_CARD:
        {
            return std::make_unique<UpgradeCard>();
        }
        default:
            return nullptr;
    }
}

std::unordered_map<CardType, int> SpellCardFactory::m_cardsCosts = {
    {CardType::FIREBALL, CostCardsConfig::FireBall},
    {CardType::EARTHQUAKE, CostCardsConfig::EarthQuake},
    {CardType::POISON_TRAP, CostCardsConfig::PoisonTrap},
    {CardType::SUMMON_ARCHER, CostCardsConfig::SummonArcher},
    {CardType::UPGRADE_CARD, CostCardsConfig::Upgrade}
};
