#include "poisontrapcard.h"
#include "gamemodel.h"

PoisonTrapCard::PoisonTrapCard()
    :
    m_trapDamage(SpellConfig::PoisonTrapDamage) , m_wasUsed(false) {}

void PoisonTrapCard::useCard(ISpellConfig& config)
{
    config.execute(*this);
}

void PoisonTrapCard::upgrade(int level)
{
    m_trapDamage += level * 5;
    m_upgradeLevel = level;
}

void PoisonTrapCard::clearUpgrades()
{
    m_trapDamage = SpellConfig::PoisonTrapDamage;
    m_wasUsed = false;
    m_upgradeLevel = 0;
}

void PoisonTrapCard::apply(PlayerSpellConfig &config)
{
    auto& board = config.getBoard();
    std::pair<int,int> targetPos = config.getTargetPos();

    if (board.isValidCell(targetPos.first, targetPos.second))
    {
        board.currentCell(targetPos.first, targetPos.second).setType(CellType::POISON_TRAP);
        auto observer = std::make_unique<PoisonTrap>(m_trapDamage, config.getEnemies(), targetPos.first, targetPos.second);
        board.addObserver(std::move(observer));
        EventDispatcher::instance().notify(NewCellEvent(CellType::POISON_TRAP, targetPos));
        m_wasUsed = true;
    }
}

void PoisonTrapCard::apply(EnemySpellConfig&)
{
    throw std::logic_error("PoisonTrapCard: Enemy spell config not supported yet");
}
