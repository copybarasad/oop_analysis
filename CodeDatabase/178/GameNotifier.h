#ifndef GAME_NOTIFIER_H
#define GAME_NOTIFIER_H

#include "Position.h"
#include <string>

class IGameNotifier {
public:
    virtual ~IGameNotifier() = default;
    
    virtual void notifyPlayerMoved(const Position& from, const Position& to) = 0;
    virtual void notifyPlayerAttacked(const std::string& target, int damage) = 0;
    virtual void notifyPlayerDamaged(int damage, const std::string& source) = 0;
    virtual void notifyEnemyKilled(const Position& position, int coinsReward) = 0;
    virtual void notifyTowerDestroyed(const Position& position) = 0;
    virtual void notifySpellUsed(const std::string& spellName, const Position& target) = 0;
    virtual void notifyCommandExecuted(const std::string& command) = 0;
};

#endif