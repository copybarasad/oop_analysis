#include "logger.h"
#include <QDebug>

Logger::Logger()
{
    EventDispatcher::instance().subscribe<MoveEvent>(
        [this](const MoveEvent &e){ onMove(e); });

    EventDispatcher::instance().subscribe<AttackEvent>(
        [this](const AttackEvent &e){ onAttack(e); });

    EventDispatcher::instance().subscribe<DeadEvent>(
        [this](const DeadEvent &e){ onDead(e); });

    EventDispatcher::instance().subscribe<BuyCardEvent>(
        [this](const BuyCardEvent &e){ onBuyCard(e); });

    EventDispatcher::instance().subscribe<UseSpellEvent>(
        [this](const UseSpellEvent &e){ onUseSpell(e); });

    EventDispatcher::instance().subscribe<LevelLoadedEvent>(
        [this](const LevelLoadedEvent &e){ onLevelLoaded(e); });

    EventDispatcher::instance().subscribe<NewCellEvent>(
        [this](const NewCellEvent &e){ onNewCell(e); });

    EventDispatcher::instance().subscribe<WinEvent>(
        [this](const WinEvent &e){ onWin(e); });
}

void Logger::onMove(const MoveEvent &e)
{
    qDebug()
    << "[Move]"
    << "actor=" << (int)e.m_actor
    << "from=(" << e.m_oldPos.first << "," << e.m_oldPos.second << ")"
    << "to=("   << e.m_newPos.first << "," << e.m_newPos.second << ")"
    << "oldCell=" << (int)e.m_oldCell;
}

void Logger::onAttack(const AttackEvent &e)
{
    qDebug()
    << "[Attack]"
    << "attacker=" << (int)e.m_attacker
    << "target="   << (int)e.m_target
    << "damage="   << e.m_damage
    << "oldHealth=" << e.m_oldHealth;
}

void Logger::onDead(const DeadEvent &e)
{
    qDebug()
    << "[Dead]"
    << "actor=" << (int)e.m_actor
    << "pos=(" << e.m_targetPos.first << "," << e.m_targetPos.second << ")";
}

void Logger::onBuyCard(const BuyCardEvent &e)
{
    qDebug()
    << "[BuyCard]"
    << "card=" << (int)e.m_card
    << "newCount=" << e.m_newCount;
}

void Logger::onUseSpell(const UseSpellEvent &e)
{
    qDebug()
    << "[UseSpell]"
    << "card=" << (int)e.m_card
    << "newCount=" << e.m_newCount;
}

void Logger::onLevelLoaded(const LevelLoadedEvent &e)
{
    qDebug()
    << "[LevelLoaded]"
    << "newBoardSize=" << e.m_newBoardSize;
}

void Logger::onNewCell(const NewCellEvent &e)
{
    qDebug()
    << "[NewCell]"
    << "type=" << (int)e.m_type
    << "coord=(" << e.m_coord.first << "," << e.m_coord.second << ")";
}

void Logger::onWin(const WinEvent &e)
{
    qDebug()
    << "[Win]"
    << "points=" << e.m_points;
}
