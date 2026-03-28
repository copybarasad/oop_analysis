#ifndef EVENTFACTORY_H
#define EVENTFACTORY_H

#include <memory>
#include <string>
#include "gameevent.h"

class EventFactory {
public:
    static std::shared_ptr<GameEvent> createPlayerMoved(int fromX, int fromY, int toX, int toY);
    static std::shared_ptr<GameEvent> createPlayerAttacked(const std::string& target, int damage, int x, int y);
    static std::shared_ptr<GameEvent> createPlayerDamaged(int damage, const std::string& source);
    static std::shared_ptr<GameEvent> createSpellCast(const std::string& spellName, int cost, int x, int y);
    static std::shared_ptr<GameEvent> createEnemyDied(const std::string& enemyType, int x, int y);
    static std::shared_ptr<GameEvent> createMoneyChanged(int amount, int newBalance);
    static std::shared_ptr<GameEvent> createCardReceived(const std::string& cardName);
    static std::shared_ptr<GameEvent> createGameStarted();
    static std::shared_ptr<GameEvent> createLevelStarted(int level);
    static std::shared_ptr<GameEvent> createPlayerHealed(int amount);
    static std::shared_ptr<GameEvent> createEnemyDamaged(int damage);
    static std::shared_ptr<GameEvent> createEnemyAttacked(int damage);
    static std::shared_ptr<GameEvent> createTowerDamaged(int damage);
    static std::shared_ptr<GameEvent> createTowerDestroyed();
    static std::shared_ptr<GameEvent> createLevelCompleted();
    static std::shared_ptr<GameEvent> createScoreChanged(int newScore);
};

#endif
