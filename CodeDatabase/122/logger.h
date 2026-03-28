#ifndef LOGGER_H
#define LOGGER_H

#include "events.h"
#include "eventdispatcher.h"

class Logger
{
public:
    Logger();

private:
    void onMove(const MoveEvent &e);
    void onAttack(const AttackEvent &e);
    void onDead(const DeadEvent &e);
    void onBuyCard(const BuyCardEvent &e);
    void onUseSpell(const UseSpellEvent &e);
    void onLevelLoaded(const LevelLoadedEvent &e);
    void onNewCell(const NewCellEvent &e);
    void onWin(const WinEvent &e);
};

#endif // LOGGER_H
